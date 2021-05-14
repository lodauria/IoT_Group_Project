#ifndef IOTGROUPPROJECT_BOATPRESENCEESTIMATOR_H
#define IOTGROUPPROJECT_BOATPRESENCEESTIMATOR_H

#include "srf04.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "stdbool.h"

#define DEFAULT_HYSTERESIS_MM 500
#define DEFAULT_BOAT_DISTANCE_THRESHOLD_MM 750

typedef struct boatPresenceEstimator_s {
    srf04_t srf04;
    srf04_params_t srf04Params;
    bool lastValid;
}boatPresenceEstimator_t;

int boat_presence_estimator_init(boatPresenceEstimator_t *boatPresenceEstimator, gpio_t triggerPin, gpio_t echoPin);
int boat_presence_estimator_get_present(boatPresenceEstimator_t *boatPresenceEstimator, int numberOfRead);

#endif //IOTGROUPPROJECT_BOATPRESENCEESTIMATOR_H
