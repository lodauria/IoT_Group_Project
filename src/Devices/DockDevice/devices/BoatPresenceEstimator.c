#include "BoatPresenceEstimator.h"
#include "stdint.h"
#include "../../Common/conversion.h"
#include "random.h"

#define FAKE_SENSOR_UPDATE_INTERVAL_S 60*3
#ifdef FAKE_SENSOR
uint32_t lastUpdate;
#endif
#define NUMBER_OF_READ 5        //Use an odd value
#define MEASUREMENT_INTERVAL_MS 50

int boat_presence_estimator_init(boatPresenceEstimator_t *boatPresenceEstimator, gpio_t triggerPin) {
#ifdef FAKE_SENSOR
    random_init(xtimer_now_usec());
    lastUpdate = 0;
    printf("Fake sensor initialized\n");
    (void)boatPresenceEstimator;
    (void)triggerPin;
#else
    boatPresenceEstimator->pin = triggerPin;
    gpio_init(triggerPin, GPIO_IN_PU);
#endif
    return 1;
}

int boat_presence_estimator_get_present(boatPresenceEstimator_t *boatPresenceEstimator) {
    //Return 1 if the boat is present
    //Return 0 if the boat is not present
#ifdef FAKE_SENSOR
    if(US2S(xtimer_now_usec() - lastUpdate) > FAKE_SENSOR_UPDATE_INTERVAL_S){
        //Fake sensor is updated every 3 minutes
        boatPresenceEstimator->lastRead = random_uint32_range(0,10) > 5;
    }
#else
    // It get NUMBER_OF_READ measurements, one every MEASUREMENT_INTERVAL_MS, the most frequent is returned
    uint8_t trueValue = 0;
    uint8_t falseValue = 0;
    for (uint8_t i = 0; i < NUMBER_OF_READ; i++) {
        if(gpio_read(boatPresenceEstimator->pin) > 0)
            trueValue++;
        else
            falseValue++;
        xtimer_usleep(MS2US(MEASUREMENT_INTERVAL_MS));
    }
    boatPresenceEstimator->lastRead = trueValue > falseValue;
#endif
    return boatPresenceEstimator->lastRead;
}
