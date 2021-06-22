#include "BoatPresenceEstimator.h"
#include "stdint.h"
#include "../../Common/conversion.h"
#include "random.h"

#define FAKE_SENSOR_UPDATE_INTERVAL_S 60*3
#ifdef FAKE_SENSOR
uint32_t lastUpdate;
#endif

int boat_presence_estimator_init(boatPresenceEstimator_t *boatPresenceEstimator, gpio_t triggerPin) {
#ifdef FAKE_SENSOR
    random_init(xtimer_now_usec());
    lastUpdate = 0;
    printf("Fake sensor initialized\n");
#else
    boatPresenceEstimator->pin = triggerPin;
    gpio_init(triggerPin,GPIO_IN_PU);
#endif
    return 1;
}

int boat_presence_estimator_get_present(boatPresenceEstimator_t *boatPresenceEstimator) {
    //Return 1 if the boat is present
    //Return 0 if the boat is not present
#ifdef FAKE_SENSOR
    if(US2S(xtimer_now_usec() - lastUpdate) > FAKE_SENSOR_UPDATE_INTERVAL_S){
        //Fake sensor is updated every 3 minutes
        return random_uint32_range(0,10) > 5;
    }
#else
    return gpio_read(boatPresenceEstimator->pin) > 0;
#endif
}
