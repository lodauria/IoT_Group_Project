#include <stdio.h>

#include "xtimer.h"
#include "periph/gpio.h"
#include "devices/BoatPresenceEstimator.h"


#define TRIGGER_PIN GPIO_PIN(PORT_B,3)  //D3
#define ECHO_PIN GPIO_PIN(PORT_A,10)    //D2
#define LED_PIN GPIO_PIN(PORT_B,5)      //D2

boatPresenceEstimator_t boatPresenceEstimator;
gpio_t led = LED_PIN;

int main(void) {
    xtimer_init();
    printf("Hello from RIOT");
    boat_presence_estimator_init(&boatPresenceEstimator, TRIGGER_PIN, ECHO_PIN);
    gpio_init(led, GPIO_OUT);

    while (1) {
        int isPresent = boat_presence_estimator_get_present(&boatPresenceEstimator, 3);
        printf("Is boat present %d\n", isPresent);
        gpio_write(led, isPresent > 0 ? 1 : 0);
        xtimer_sleep(1);
    }


    return 0;
}
