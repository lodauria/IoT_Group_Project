#ifndef RIOT_GREENHOUSE_EMCUTE_CONNECTION_H
#define RIOT_GREENHOUSE_EMCUTE_CONNECTION_H

#include "stddef.h"
#include "stdint.h"
#include "../RIOT/sys/include/net/emcute.h"
#include "net/emcute.h"

#define TOPIC_MAXLEN        256
#define MAX_TOPIC           8

typedef struct topic_subscription_s {
    emcute_sub_t subscription;
    char enabled;
} topic_subscription_t;

typedef struct topic_publisher_s {
    emcute_topic_t emcute_topic;
    char enabled;
} topic_publisher_s;

typedef struct emcute_manager_s {
    topic_publisher_s topicPublisher[MAX_TOPIC];
    topic_subscription_t topicSubscription[MAX_TOPIC];
    uint16_t node_id;
    char node_name[64];
    char topics[TOPIC_MAXLEN];
    uint8_t connected;
} emcute_manager_t;

int emcuteManagerSetConnection(emcute_manager_t *emcuteManager, const char *server_addr, int mqtt_port, int node_id);

void emcuteManagerSubscribeTopic(emcute_manager_t *emcuteManager, const char *mqtt_topic,
                                 void(*on_message)(const emcute_topic_t *, void *, size_t));

void emcuteManagerRegisterPublishTopic(emcute_manager_t *emcuteManager, const char *mqtt_topic);

void emcuteManagerPublish(emcute_manager_t *emcuteManager, char *topicName, char *str);

int emcuteManagerGetNodeId(emcute_manager_t *emcuteManager);

int emcuteManagerIsConnected(emcute_manager_t *emcuteManager);

void emcuteManagerInit(emcute_manager_t *emcuteManager, int node_id);


#endif
