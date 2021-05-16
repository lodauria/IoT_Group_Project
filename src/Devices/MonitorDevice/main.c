#include <stdio.h>
#include "devices/ssd1306.h"
#include "xtimer.h"

int main(void) {
    ssd1306_t display;

    printf("Hello from Monitor Device\n");
    ssd1306_init(&display,0x3C);
    xtimer_init();
    while(1) {
        ssd1306_writeArrow(&display, LEFT);
        xtimer_sleep(1);
        ssd1306_writeArrow(&display, TOP_LEFT);
        xtimer_sleep(1);
        ssd1306_writeArrow(&display, TOP);
        xtimer_sleep(1);
        ssd1306_writeArrow(&display, TOP_RIGHT);
        xtimer_sleep(1);
        ssd1306_writeArrow(&display, RIGHT);
        xtimer_sleep(1);
        ssd1306_writeArrow(&display, BOTTOM_RIGHT);
        xtimer_sleep(1);
        ssd1306_writeArrow(&display, BOTTOM);
        xtimer_sleep(1);
        ssd1306_writeArrow(&display, BOTTOM_LEFT);
        xtimer_sleep(1);
    }

    return 0;
}
