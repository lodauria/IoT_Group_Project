#include <stdio.h>

#include "xtimer.h"
#include "devices/BoatPresenceEstimator.h"

#define TRIGGER_PIN GPIO_PIN(PORT_B,3)  //D3
#define ECHO_PIN GPIO_PIN(PORT_A,10)    //D2

boatPresenceEstimator_t boatPresenceEstimator;

int main(void) {
    xtimer_init();
    printf("Hello from RIOT");
    boat_presence_estimator_init(&boatPresenceEstimator, TRIGGER_PIN, ECHO_PIN);

    while (1) {
        printf("Is boat present %d\n", boat_presence_estimator_get_present(&boatPresenceEstimator,3));
        xtimer_sleep(1);
    }


    return 0;
}
