#include "my_scheduler.h"
#include "thread.h"
#include "stdint.h"

#include "../RIOT/sys/include/xtimer.h"
#include "logic_condition.h"
#include "convertion.h"

#define MAX_FUNCTION 4

char stack[THREAD_STACKSIZE_MAIN];

int analog_device_last = -1;
uint32_t last_analog_time_scan = 0;

scheduled_function_t functions[MAX_FUNCTION];

void *scheduler_core(void *arg) {
    (void) arg;

    while (1) {
        for (int i = 0; i < MAX_FUNCTION; i++) {
            if (functions[i].active && US2MS(xtimer_now_usec()) > functions[i].next_schedule) {
                functions[i].function();
                functions[i].next_schedule = US2MS(xtimer_now_usec()) + functions[i].interval;
            }
        }
        xtimer_msleep(15);
    }
    return NULL;
}

void scheduler_init(void) {
    xtimer_init();
    for (int i = 0; i < MAX_FUNCTION; i++) {
        functions[i].active = 0;
    }
}

void scheduler_add_function(uint16_t interval, void (*function)(void)) {
    for (int i = 0; i < MAX_FUNCTION; i++) {
        if (functions[i].active == 0) {
            functions[i].interval = interval;
            functions[i].function = function;
            functions[i].next_schedule = US2MS(xtimer_now_usec()) + interval;
            functions[i].active = 1;
            break;
        }
    }
}

void scheduler_start(void) {
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  scheduler_core,
                  NULL, "Scheduler code");
}