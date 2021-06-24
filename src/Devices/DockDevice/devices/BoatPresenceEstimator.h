#ifndef IOTGROUPPROJECT_BOATPRESENCEESTIMATOR_H
#define IOTGROUPPROJECT_BOATPRESENCEESTIMATOR_H

#include "srf04.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "stdbool.h"

#define DEFAULT_HYSTERESIS_MM 500
#define DEFAULT_BOAT_DISTANCE_THRESHOLD_MM 750

typedef struct boatPresenceEstimator_s {
    gpio_t pin;
    uint8_t lastRead;
}boatPresenceEstimator_t;

int boat_presence_estimator_init(boatPresenceEstimator_t *boatPresenceEstimator, gpio_t triggerPin);
int boat_presence_estimator_get_present(boatPresenceEstimator_t *boatPresenceEstimator);

#endif //IOTGROUPPROJECT_BOATPRESENCEESTIMATOR_H
