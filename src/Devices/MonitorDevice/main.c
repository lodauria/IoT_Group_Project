#include <stdio.h>
#include "devices/ssd1306.h"
#include "xtimer.h"

int main(void) {
    ssd1306_t display;

    ssd1306_init(&display, 0x3C);
    xtimer_init();
    ssd1306_addIndication(&display, LEFT, "LUCA");
    xtimer_sleep(5);
    ssd1306_removeIndication(&display,"LUCA");
    xtimer_sleep(5);
    ssd1306_addIndication(&display,BOTTOM_LEFT,"TS3433");
    while(1);
    return 0;
}
