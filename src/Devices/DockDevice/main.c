#include <stdio.h>

#include "../Common/logic_condition.h"
#include "../Common/my_scheduler.h"

int main(void)
{
    printf("Hello from RIOT");

    logic_condition_init();
    scheduler_init();
    return 0;
}
