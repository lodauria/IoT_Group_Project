#ifndef RIOT_GREENHOUSE_EMCUTE_CONNECTION_H
#define RIOT_GREENHOUSE_EMCUTE_CONNECTION_H

#include "stddef.h"
#include "stdint.h"
#include "../RIOT/sys/include/net/emcute.h"
#include "net/emcute.h"

#define TOPIC_MAXLEN        256

typedef struct emcute_manager_s {
    emcute_topic_t emcute_topic;
    emcute_sub_t subscription;
    uint16_t node_id;
    char node_name[64];
    char topics[TOPIC_MAXLEN];
    uint8_t connected;
} emcute_manager_t;

int emcuteManagerSetConnection(emcute_manager_t *emcuteManager, const char *server_addr, const char *mqtt_topic, int mqtt_port,
                              void(*on_message)(const emcute_topic_t *, void *, size_t));
void emcuteManagerPublish(emcute_manager_t * emcuteManager, char *str);
int emcuteManagerGetNodeId(emcute_manager_t * emcuteManager);
int emcuteManagerIsConnected(emcute_manager_t * emcuteManager);
void emcuteManagerInit(emcute_manager_t *emcuteManager, int node_id);


#endif
