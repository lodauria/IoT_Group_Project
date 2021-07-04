#include <stdio.h>
#include "devices/ssd1306.h"
#include "xtimer.h"
#include "stdlib.h"
#include "string.h"
#include "msg.h"
#include "../Common/network_manager.h"

#ifdef USE_ETHOS
#include "shell.h"
#include "net/emcute.h"
#endif

#include "jsmn.h"
#include "devices/ssd1306.h"

#define MAIN_QUEUE_SIZE     (8)
#define MAX_JSON_TOKEN      128

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
emcute_manager_t emcuteManager;
jsmn_parser p;
jsmntok_t t[MAX_JSON_TOKEN];
ssd1306_t display;

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

arrowDirection_e getDirectionByDockId(int dockId) {
    int monitorId = emcuteManagerGetNodeId(&emcuteManager);
    switch (monitorId) {
        case 1:
            if (dockId <= 10)
                return TOP_RIGHT;
            else if (11 <= dockId && dockId <= 15)
                return LEFT;
            else
                return TOP;
        case 2:
            if (dockId <= 10)
                return RIGHT;
            else if (11 <= dockId && dockId <= 20)
                return BOTTOM_LEFT;
            else if (dockId <= 25)
                return TOP_LEFT;
            else
                return TOP;
        case 3:
            if (dockId <= 10)
                return RIGHT;
            else if (dockId >= 20)
                return BOTTOM_LEFT;
            else
                return TURN_AROUND;
        case 4:
            if (dockId <= 10)
                return BOTTOM;
            else if (dockId >= 20)
                return LEFT;
            else if (dockId >= 11 && dockId <= 15)
                return RIGHT;
            else
                return TURN_AROUND;
        default:
            return OFF;
    }

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

    if (strlen(boat_id) > 0 && strlen(dock_num) > 0 && strlen(event) > 0) {
        const int dock_num_integer = atoi(dock_num);
        const int event_integer = atoi(event);
        switch (event_integer) {
            case 0:
                arrowDirection_e arrow = getDirectionByDockId(dock_num_integer);
                if (arrow != OFF)
                    ssd1306_addIndication(&display, arrow, boat_id);
                break;
            case 1:
                ssd1306_removeIndication(&display, boat_id);
                break;
            case 2:
                if (DEFAULT_NODE == 1)//If it is the monitor at the entrance
                    ssd1306_addIndication(&display, NO_RESERVATION, boat_id);
                break;
            default:
                break;
        }
    }
}

#ifdef USE_ETHOS
void on_received_message(const emcute_topic_t *topic, void *data, size_t len) {

    char *topic_name = MQTT_TOPIC;
    char *in = (char *) data;


    if (strcmp(topic->name, topic_name) != 0)
        return;

    printf("### got publication for topic '%s' [%i] ###\n",
           topic->name, (int) topic->id);

    for (size_t i = 0; i < len; i++) {
        printf("%c", in[i]);
    }
    puts("\n");
    process_message(data,len);
}

#endif

int main(void) {
    ssd1306_init(&display, 0x3C);
    // we need a message queue for the thread running the shell in order to
    // receive potentially fast incoming networking packets
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("RIOT network stack example application");

#ifdef USE_ETHOS
    emcuteManagerSetConnection(&emcuteManager, ETHOS_IP, MQTT_PORT, DEFAULT_NODE);
    emcuteManagerSubscribeTopic(&emcuteManager,MQTT_TOPIC,on_received_message);
#else
    loraSetConnection(&emcuteManager, DEFAULT_NODE, process_message);
#endif

    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}
