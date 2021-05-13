//
// Created by Luca Giovannesi.
//

#ifndef LOGIC_CONDITION_H
#define LOGIC_CONDITION_H

#include "stdlib.h"
#include "../RIOT/sys/include/xtimer.h"

typedef enum {
    LESS,
    GREATER,
    EQUAL,
    NOT_EQUAL
} operator_e;


typedef struct logic_condition_s {
    int enabled;
    const int *operand_a;
    const int *operand_b;
    operator_e operator;
    int last_evaluation;
    time_t next_schedule;
    time_t interval;

    const int *parameter;

    void (*action)(int *);

    const void *next;
} logic_condition_t;

void logic_condition_init(void);

const logic_condition_t *
logic_condition_add(const int *operand_a, operator_e operator, const int *operand_b, void(*action)(int *),
                    const int *parameter,
                    const logic_condition_t *next);
void logic_condition_set_interval(const logic_condition_t *condition, time_t interval_ms);

void logic_condition_update(void);

#endif //LOGIC_CONDITION_H
