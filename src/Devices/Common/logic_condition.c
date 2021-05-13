//
// Created by Luca Giovannesi.
//

#include "logic_condition.h"

#include "convertion.h"

#define MAX_LOGIC_CONDITION 16

logic_condition_t logic_conditions[MAX_LOGIC_CONDITION];

void logic_condition_init(void) {
    xtimer_init();
}

const logic_condition_t *logic_condition_add(const int *operand_a,
                                             operator_e operator,
                                             const int *operand_b,
                                             void(*action)(int *),
                                             const int *parameter,
                                             const logic_condition_t *next) {
    for (int i = 0; i < MAX_LOGIC_CONDITION; i++) {
        if (!logic_conditions[i].enabled) {
            logic_conditions[i].operand_a = operand_a;
            logic_conditions[i].operand_b = operand_b;
            logic_conditions[i].operator = operator;
            logic_conditions[i].action = action;
            logic_conditions[i].enabled = 1;
            logic_conditions[i].next = next;
            logic_conditions[i].parameter = parameter;
            logic_conditions[i].last_evaluation = 0;
            logic_conditions[i].interval = 0;
            logic_conditions[i].next_schedule = 0;
            return (logic_condition_t *) (logic_conditions + i);
        }
    }
    return NULL;
}

static int _logic_condition_evaluate_condition(const logic_condition_t *condition) {
    if (condition == NULL)
        return 1;
    else
        return condition->last_evaluation &&
               _logic_condition_evaluate_condition((const logic_condition_t *) condition->next);

}

static int _logic_condition_evaluate_operand(const int *operand_a, const int *operand_b, operator_e operator) {
    switch (operator) {
        case GREATER:
            return *operand_a > *operand_b;
        case LESS:
            return *operand_a < *operand_b;
        case EQUAL:
            return *operand_a == *operand_b;
        case NOT_EQUAL:
            return *operand_a != *operand_b;
        default:
            return 0;
    }
}

void logic_condition_set_interval(const logic_condition_t *condition, time_t interval_ms) {
    logic_condition_t *cond = (logic_condition_t *) condition;
    cond->interval = interval_ms;
    cond->next_schedule = US2MS(xtimer_now_usec()) + interval_ms;
}

#include "stdio.h"

void logic_condition_update(void) {
    for (int i = 0; i < MAX_LOGIC_CONDITION; i++) {
        if (logic_conditions[i].enabled) {
            //If the logic condition must be scheduled with an interval
            const int must_be_scheduled = logic_conditions[i].interval == 0 ||
                                          US2MS(xtimer_now_usec()) > logic_conditions[i].next_schedule;

            if (must_be_scheduled) {
                //Update the next schedule if needed
                if (logic_conditions[i].interval != 0) {
                    //For timer scheduled lc, update the next schedule timer
                    logic_conditions[i].next_schedule = US2MS(xtimer_now_usec()) + logic_conditions[i].interval;
                }

                //Check if current lc hold
                const int current_lc_evaluation =
                        _logic_condition_evaluate_operand(logic_conditions[i].operand_a, logic_conditions[i].operand_b,
                                                          logic_conditions[i].operator);
                //check if next lc hold
                const int next_lc_evaluation = _logic_condition_evaluate_condition(logic_conditions[i].next);

                logic_conditions[i].last_evaluation = current_lc_evaluation && next_lc_evaluation;


                if (logic_conditions[i].last_evaluation && logic_conditions[i].action != NULL) {
                    //If there is an action, do it
                    logic_conditions[i].action((int *) logic_conditions[i].parameter);
                }
            }
        }
    }
}




