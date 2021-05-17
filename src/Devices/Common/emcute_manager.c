#include "emcute_manager.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


#include "net/ipv6/addr.h"
#include "msg.h"

#define MAX_JSON_TOKEN      128

char em_stack[THREAD_STACKSIZE_MAIN];

void emcuteManagerPublish(emcute_manager_t *emcuteManager, char *str) {
    if (emcuteManager->connected) {
        unsigned flags = EMCUTE_QOS_0;
        int res = emcute_pub(&emcuteManager->emcute_topic, str, strlen(str), flags);
        if (res != EMCUTE_OK) {
            printf("Error while publishing %d\n", res);
        }
    }
}

int emcuteManagerGetNodeId(emcute_manager_t *emcuteManager) {
    return emcuteManager->node_id;
}

static void *emcute_thread(void *arg) {
    char *nodeName = arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, nodeName);
    return NULL;   // should never be reached
}

int emcuteManagerIsConnected(emcute_manager_t *emcuteManager) {
    return emcuteManager->connected;
}

void emcuteManagerInit(emcute_manager_t *emcuteManager, int node_id) {
    memset(&(emcuteManager->subscription), 0, sizeof(emcute_sub_t));
    memset(emcuteManager->node_name, 0, 64);
    sprintf(emcuteManager->node_name, "node%d", node_id);
    emcuteManager->connected = 0;
    emcuteManager->node_id = node_id;

    thread_create(em_stack, sizeof(em_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  emcute_thread,
                  ((void *) emcuteManager->node_name), "emcute thread");

}

int emcuteManagerSetConnection(emcute_manager_t *emcuteManager, const char *server_addr, const char *mqtt_topic,
                               int mqtt_port,
                               void(*on_message)(const emcute_topic_t *, void *, size_t)) {

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

    //setup subscription to topic
    unsigned flags = EMCUTE_QOS_0;
    emcuteManager->subscription.cb = on_message;
    memset(emcuteManager->topics, 0, TOPIC_MAXLEN);
    emcuteManager->subscription.topic.name = mqtt_topic;

    if (emcute_sub(&(emcuteManager->subscription), flags) != EMCUTE_OK) {
        printf("error: unable to subscribe to %s\n", mqtt_topic);
        return 0;
    }
    else
        printf("Now subscribed to %s\n", mqtt_topic);

    //Register topic to send
    emcuteManager->emcute_topic.name = mqtt_topic;
    emcute_reg(&(emcuteManager->emcute_topic));
    emcuteManager->connected = 1;

    return 1;
}
