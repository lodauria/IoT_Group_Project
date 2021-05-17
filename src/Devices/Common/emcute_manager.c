#include "emcute_manager.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


#include "net/ipv6/addr.h"
#include "msg.h"

#define MAX_JSON_TOKEN      128

char em_stack[THREAD_STACKSIZE_MAIN];

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