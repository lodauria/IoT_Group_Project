#include <stdio.h>

#include "xtimer.h"
#include "periph/gpio.h"
#include "devices/BoatPresenceEstimator.h"
#include "../Common/emcute_manager.h"
#include "jsmn.h"
#include "shell.h"
#include "msg.h"
#include "../Common/emcute_manager.h"
#include "net/emcute.h"

#define TRIGGER_PIN GPIO_PIN(PORT_B,3)  //D3
#define ECHO_PIN GPIO_PIN(PORT_A,10)    //D2
#define LED_PIN GPIO_PIN(PORT_B,5)      //D4

#define MAIN_QUEUE_SIZE     (8)
#define MAX_JSON_TOKEN      128
#define MAX_MSG_SIZE        128

boatPresenceEstimator_t boatPresenceEstimator;
gpio_t led = LED_PIN;

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
emcute_manager_t emcuteManager;
jsmn_parser p;
jsmntok_t t[MAX_JSON_TOKEN];
char thread_stack[THREAD_STACKSIZE_MAIN];

static void *boat_thread(void *arg) {
    (void )arg;
    char msg[MAX_MSG_SIZE];
    while (1){
        memset(msg,0,MAX_MSG_SIZE);
        int boatIsDetected = boat_presence_estimator_get_present(&boatPresenceEstimator,5);
        sprintf(msg,"{\"dock_id\":%d,\"detected\":%d}", emcuteManagerGetNodeId(&emcuteManager),boatIsDetected);
        emcuteManagerPublish(&emcuteManager,MQTT_TOPIC_DETECT_BOAT,msg);
        xtimer_sleep(MQTT_PUBLISH_INTERVAL_S);
    }
    return NULL;   // should never be reached
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

void on_received_message(const emcute_topic_t *topic, void *data, size_t len) {

    char *topic_name = MQTT_TOPIC_LED;
    char *in = (char *) data;


    if (strcmp(topic->name, topic_name) != 0)
        return;

    printf("### got publication for topic '%s' [%i] ###\n",
           topic->name, (int) topic->id);

    /*
    for (size_t i = 0; i < len; i++) {
        printf("%c", in[i]);
    }
    puts("\n");
     */

    memset(&p, 0, sizeof(jsmn_parser));
    memset(&t, 0, sizeof(jsmntok_t) * MAX_JSON_TOKEN);

    jsmn_init(&p);
    int r = jsmn_parse(&p, in, len, t, MAX_JSON_TOKEN);

    if (r < 0) {
        printf("Failed to parse JSON: %d\n", r);
        return;
    }

    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("Object expected\n");
        return;
    }

    //Loop over all keys of the root objects
    char state[8];
    char dock_id[8];
    for (int i = 1; i < r; i++) {
        if (jsoneq(in, &t[i], "dock_id") == 0) {
            sprintf(dock_id, "%.*s", t[i + 1].end - t[i + 1].start,
                    in + t[i + 1].start);
            i++;
        }
        else if (jsoneq(in, &t[i], "state") == 0) {
            sprintf(state, "%.*s", t[i + 1].end - t[i + 1].start,
                    in + t[i + 1].start);
            i++;
        }
        else {
            printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
                   in + t[i].start);
        }
    }

    if (strlen(dock_id) > 0 && strlen(state) > 0) {

        const int state_integer = atoi(state);
        const int dock_id_integer = atoi(dock_id);

        if (dock_id_integer == emcuteManagerGetNodeId(&emcuteManager)) {
            gpio_write(led, state_integer);
            printf("setted %d\n", state_integer);

        }
    }
}

int connect_mqtt(int argc, char **argv) {
    (void) argc;
    (void) argv;
    if (argc > 2) {
        const int nodeId = atoi(argv[2]);
        emcuteManagerSetConnection(&emcuteManager, argv[1], MQTT_PORT, nodeId);
        emcuteManagerSubscribeTopic(&emcuteManager, MQTT_TOPIC_LED, on_received_message);
        emcuteManagerRegisterPublishTopic(&emcuteManager,MQTT_TOPIC_DETECT_BOAT);
    }
    else {
        printf("Usage: %s <broker_addr> <node_id>\n", argv[0]);
    }
    return 0;
}

static const shell_command_t shell_commands[] = {

#ifndef USE_ETHOS
        {"connect", "Connect to the UDP server", connect_mqtt},
#endif
        {NULL, NULL, NULL}
};

int main(void) {
    xtimer_init();
    boat_presence_estimator_init(&boatPresenceEstimator, TRIGGER_PIN, ECHO_PIN);
    gpio_init(led, GPIO_OUT);

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("RIOT network stack example application");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
#ifdef USE_ETHOS
    emcuteManagerSetConnection(&emcuteManager, ETHOS_IP, MQTT_PORT, DEFAULT_NODE);
    emcuteManagerSubscribeTopic(&emcuteManager,MQTT_TOPIC_LED,on_received_message);
    emcuteManagerRegisterPublishTopic(&emcuteManager,MQTT_TOPIC_DETECT_BOAT);
#endif

    thread_create(thread_stack, sizeof(thread_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  boat_thread,
                  NULL, "emcute thread");

    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
