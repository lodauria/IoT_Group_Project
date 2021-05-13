//
// Created by Luca Giovannesi.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../RIOT/sys/include/xtimer.h"

typedef struct scheduled_function_s{
    uint32_t next_schedule;
    uint32_t interval;
    void (*function)(void);
    uint8_t active;
}scheduled_function_t;

void scheduler_init(void);
void scheduler_add_function(uint16_t intervalms, void (*function)(void));
void scheduler_start(void);

#endif //SCHEDULER_H
