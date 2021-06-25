#include <stdio.h>

#include "xtimer.h"
#include "periph/gpio.h"
#include "devices/dock.h"
#include "jsmn.h"
#include "shell.h"
#include "msg.h"
#include "string.h"
#include "stdlib.h"
#include "../Common/network_manager.h"


#ifdef USE_ETHOS
#include "net/emcute.h"
#endif

#define MAIN_QUEUE_SIZE     (8)
#define MAX_JSON_TOKEN      128
#define MAX_MSG_SIZE        128

#ifdef USE_ETHOS
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
#endif
emcute_manager_t emcuteManager;

//DOCK DEVICE DEFINITION
static const dockSetting_t settings[] = {
        //{DockId, SwitchPin, LedPin}
        {1, GPIO_PIN(PORT_B, 3), GPIO_PIN(PORT_B, 5)},  //D3 - D4
        {2, GPIO_PIN(PORT_B, 4), GPIO_PIN(PORT_B, 10)}, //D5 - D6
        {3, GPIO_PIN(PORT_A, 8), GPIO_PIN(PORT_A, 9)}   //D7 - D8
};

#define DOCKS_COUNT (int)(sizeof(settings) / sizeof(dockSetting_t))
dock_t docks[DOCKS_COUNT];

jsmn_parser p;
jsmntok_t t[MAX_JSON_TOKEN];
char thread_stack[THREAD_STACKSIZE_MAIN];
int isLedOn = 0;

static void *boat_thread(void *arg) {
    (void) arg;
    char msg[MAX_MSG_SIZE];
    while (1) {
        for (int i = 0; i < DOCKS_COUNT; i++) {
            dock_t *currentDock = docks + i;

            memset(msg, 0, MAX_MSG_SIZE);

            const int isDockFree = dock_get_present(currentDock);
            const uint16_t dockId = dock_get_id(currentDock);

            if (!isDockFree && dock_get_led_state(currentDock)) {
                dock_toggle_led(currentDock, 0);
            }
            if (dock_is_first_read(currentDock)) {
                dock_set_last_read(currentDock, isDockFree);
            }
            else if (isDockFree != dock_get_last_read(currentDock)) {
                //Message is sent only when state changes, NOT at once read, so i need to save the previous reads
                sprintf(msg, "{\"dock_num\":%d,\"event\":\"%d\"}", dockId, isDockFree);
                #ifdef USE_ETHOS
                emcuteManagerPublish(&emcuteManager, MQTT_TOPIC_DETECT_BOAT, msg);
                #else
                loraSendMessage(&emcuteManager, msg);
                #endif
                dock_set_last_read(currentDock, isDockFree);
                printf("Dock %u is free %d\n", dockId, isDockFree);
                #ifdef USE_ETHOS
                xtimer_sleep(MQTT_PUBLISH_INTERVAL_S - 1);
                #endif

            }
        }
        xtimer_sleep(1);
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

void process_message(void *data, size_t len) {
    char *in = (char *) data;

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
    char boat_id[64];
    char dock_num[8];
    char event[8];

    for (int i = 1; i < r; i++) {
        if (jsoneq(in, &t[i], "boat_id") == 0) {
            sprintf(boat_id, "%.*s", t[i + 1].end - t[i + 1].start,
                    in + t[i + 1].start);
            //printf("boat_id: %s\n", boat_id);
            i++;
        }
        else if (jsoneq(in, &t[i], "dock_num") == 0) {
            sprintf(dock_num, "%.*s", t[i + 1].end - t[i + 1].start,
                    in + t[i + 1].start);
            //printf("dock_id: %s\n", dock_id);
            i++;
        }
        else if (jsoneq(in, &t[i], "event") == 0) {
            sprintf(event, "%.*s", t[i + 1].end - t[i + 1].start,
                    in + t[i + 1].start);
            //printf("dock_id: %s\n", dock_id);
            i++;
        }
        else {
            printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
                   in + t[i].start);
        }
    }

    if (strlen(dock_num) > 0 && strlen(event) > 0 && strcmp(event, "0") == 0) {
        const int dock_id_integer = atoi(dock_num);
        for (int i = 0; i < DOCKS_COUNT; i++) {
            if (dock_get_id(docks + i) == dock_id_integer) {
                dock_toggle_led(docks + i, 1);
                printf("Dock %d LED turned on\n", dock_get_id(docks + i));
            }
        }
    }
}

#ifdef USE_ETHOS

void on_received_message(const emcute_topic_t *topic, void *data, size_t len) {

    char *topic_name = MQTT_TOPIC_LED;
    char *in = (char *) data;


    if (strcmp(topic->name, topic_name) != 0)
        return;

    printf("### got publication for topic '%s' [%i] ###\n",
           topic->name, (int) topic->id);

    for (size_t i = 0; i < len; i++) {
        printf("%c", in[i]);
    }
    puts("\n");
    process_message(in,len);

}

int connect_mqtt(int argc, char **argv) {
    (void) argc;
    (void) argv;
    if (argc > 2) {
        const int nodeId = atoi(argv[2]);
        emcuteManagerSetConnection(&emcuteManager, argv[1], MQTT_PORT, nodeId);
        emcuteManagerSubscribeTopic(&emcuteManager, MQTT_TOPIC_LED, on_received_message);
        emcuteManagerRegisterPublishTopic(&emcuteManager, MQTT_TOPIC_DETECT_BOAT);
    }
    else {
        printf("Usage: %s <broker_addr> <node_id>\n", argv[0]);
    }
    return 0;
}

static const shell_command_t shell_commands[] = {
        {"connect", "Connect to the UDP server", connect_mqtt},
        {NULL, NULL, NULL}
};
#endif

int main(void) {
    xtimer_init();
    for (int i = 0; i < DOCKS_COUNT; i++) {
        dock_init(docks + i, (dockSetting_t *) settings + i);
    }
#ifdef USE_ETHOS
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    emcuteManagerSetConnection(&emcuteManager, ETHOS_IP, MQTT_PORT, 0);
    emcuteManagerSubscribeTopic(&emcuteManager,MQTT_TOPIC_LED,on_received_message);
    emcuteManagerRegisterPublishTopic(&emcuteManager,MQTT_TOPIC_DETECT_BOAT);
#else
    loraSetConnection(&emcuteManager, NODE_ID, process_message);
#endif

    thread_create(thread_stack, sizeof(thread_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  boat_thread,
                  NULL, "emcute thread");
#ifdef USE_ETHOS
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
#endif
    return 0;
}
