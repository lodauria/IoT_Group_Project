#include "network_manager.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "msg.h"

#ifdef USE_ETHOS
    #include "net/ipv6/addr.h"
    char em_stack[THREAD_STACKSIZE_MAIN];
#else  //USE LORA
    #include "fmt.h"
    #include "net/loramac.h"
    #include "semtech_loramac.h"
    #include "sx127x.h"
    #include "sx127x_netdev.h"
    #include "sx127x_params.h"
    #define LORAMAC_RECV_MSG_QUEUE                   (4U)

    static msg_t _loramac_recv_queue[LORAMAC_RECV_MSG_QUEUE];
    static char _recv_stack[THREAD_STACKSIZE_DEFAULT];
    static semtech_loramac_t loramac;
    static sx127x_t sx127x;

    static uint8_t deveui[LORAMAC_DEVEUI_LEN];
    static uint8_t appeui[LORAMAC_APPEUI_LEN];
    static uint8_t appkey[LORAMAC_APPKEY_LEN];
#endif

#define MAX_JSON_TOKEN      128

#ifdef USE_ETHOS
static void *emcute_thread(void *arg) {
    char *nodeName = arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, nodeName);
    return NULL;   // should never be reached
}

void emcuteManagerPublish(emcute_manager_t *emcuteManager, char *topicName, char *str) {
    if (emcuteManager->connected) {
        for (uint8_t i = 0; i < MAX_TOPIC; i++) {
            if (emcuteManager->topicPublisher[i].enabled && strcmp(emcuteManager->topicPublisher[i].emcute_topic.name, topicName) == 0) {
                    unsigned flags = EMCUTE_QOS_0;
                    int res = emcute_pub(&(emcuteManager->topicPublisher[i].emcute_topic), str, strlen(str), flags);
                    if (res != EMCUTE_OK) {
                        printf("Error while publishing %d\n", res);
                    }
            }
        }
    }
}

void emcuteManagerSubscribeTopic(emcute_manager_t *emcuteManager, const char *mqtt_topic,
                                 void(*on_message)(const emcute_topic_t *, void *, size_t)) {
    //setup subscription to topic
    unsigned flags = EMCUTE_QOS_0;
    for (uint8_t i = 0; i < MAX_TOPIC; i++) {
        if (!emcuteManager->topicSubscription[i].enabled) {
            emcuteManager->topicSubscription[i].subscription.cb = on_message;
            emcuteManager->topicSubscription[i].subscription.topic.name = mqtt_topic;

            if (emcute_sub(&(emcuteManager->topicSubscription[i].subscription), flags) != EMCUTE_OK) {
                printf("error: unable to subscribe to %s\n", mqtt_topic);
                return;
            }
            else
                printf("Now subscribed to %s\n", mqtt_topic);

            emcuteManager->topicSubscription[i].enabled = 1;
            break;
        }
    }

}


void emcuteManagerRegisterPublishTopic(emcute_manager_t *emcuteManager, const char *mqtt_topic) {
    for (uint8_t i = 0; i < MAX_TOPIC; i++) {
        if (!emcuteManager->topicPublisher[i].enabled) {
            emcuteManager->topicPublisher[i].emcute_topic.name = mqtt_topic;
            emcute_reg(&(emcuteManager->topicPublisher[i].emcute_topic));
            emcuteManager->topicPublisher[i].enabled = 1;
            emcuteManager->connected = 1;
            break;
        }
    }
}

int emcuteManagerSetConnection(emcute_manager_t *emcuteManager, const char *server_addr, int mqtt_port, int node_id) {

    memset(emcuteManager->node_name, 0, 64);
    sprintf(emcuteManager->node_name, "node%d", node_id);
    emcuteManager->connected = 0;
    emcuteManager->node_id = node_id;

    for (uint8_t i = 0; i < MAX_TOPIC; i++) {
        emcuteManager->topicSubscription[i].enabled = 0;
        emcuteManager->topicPublisher[i].enabled = 0;
        memset(&(emcuteManager->topicPublisher[i].emcute_topic), 0, sizeof(emcute_sub_t));

    }

    thread_create(em_stack, sizeof(em_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  emcute_thread,
                  ((void *) emcuteManager->node_name), "emcute thread");

    printf("Connecting to MQTT-SN broker %s port %d.\n",
           server_addr, mqtt_port);

    sock_udp_ep_t gw = {.family = AF_INET6, .port = mqtt_port};

    char *topic = NULL;
    char *message = NULL;
    size_t len = 0;

    //parse address
    if (ipv6_addr_from_str((ipv6_addr_t * ) & gw.addr.ipv6, server_addr) == NULL) {
        printf("error parsing IPv6 address\n");
        return 0;
    }
    else
        printf("Parsed Pv6 address\n");

    if (emcute_con(&gw, true, topic, message, len, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", server_addr,
               (int) gw.port);
        return 0;
    }

    printf("Successfully connected to gateway at [%s]:%i\n",
           server_addr, (int) gw.port);
    return 1;
}
#else
// Thread for receiving LoRa messages
static void *_recv_lora_msg(void *arg){
    void(*recv_messagge)(void*,size_t) = arg;
    msg_init_queue(_loramac_recv_queue, LORAMAC_RECV_MSG_QUEUE);
    (void)arg;
    while (1) {

        // Blocks until some data is received
        if (semtech_loramac_recv(&loramac) == SEMTECH_LORAMAC_RX_DATA){
            loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
            if (loramac.rx_data.port == 10){
                recv_messagge(loramac.rx_data.payload, loramac.rx_data.payload_len);
            }
        }
    }

    return NULL;
}

int loraSetConnection(emcute_manager_t *emcuteManager, int node_id, void(*recv_messagge)(void*,size_t)) {

    memset(emcuteManager->node_name, 0, 64);
    sprintf(emcuteManager->node_name, "node%d", node_id);
    emcuteManager->connected = 0;
    emcuteManager->node_id = node_id;

    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);

    sx127x_setup(&sx127x, &sx127x_params[0], 0);
    loramac.netdev = (netdev_t * ) & sx127x;
    loramac.netdev->driver = &sx127x_driver;

    /* Initialize the loramac stack */
    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    // Use a fast datarate, e.g. BW125/SF7 in EU868
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    // Start the Over-The-Air Activation (OTAA) procedure to retrieve the
    // generated device address and to get the network and application session
    // keys.
    puts("Starting join procedure");
    int ris = semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA);
    if (ris != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        printf("Join procedure failed %d \n",ris);
        return 1;
    }
    puts("Join procedure succeeded");
    void * callback_function = recv_messagge;
    thread_create(_recv_stack, sizeof(_recv_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, _recv_lora_msg, callback_function, "recv thread");
    emcuteManager->connected = 1;

    return 1;
}

void loraSendMessage(emcute_manager_t *emcuteManager, char *message) {
    if (!emcuteManager->connected)
        return;

    printf("Sending: %s\n", message);
    /* Try to send the message */
    uint8_t ret = semtech_loramac_send(&loramac,
                                       (uint8_t *) message, strlen(message));
    if (ret != SEMTECH_LORAMAC_TX_DONE) {
        printf("Cannot send message '%s', ret code: %d\n", message, ret);
        return;
    }
}

#endif

int emcuteManagerGetNodeId(emcute_manager_t *emcuteManager) {
    return emcuteManager->node_id;
}

int emcuteManagerIsConnected(emcute_manager_t *emcuteManager) {
    return emcuteManager->connected;
}

