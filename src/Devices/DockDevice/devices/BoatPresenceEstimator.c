#include "BoatPresenceEstimator.h"


int boat_presence_estimator_init(boatPresenceEstimator_t *boatPresenceEstimator, gpio_t triggerPin, gpio_t echoPin) {
    boatPresenceEstimator->srf04Params.trigger = triggerPin;
    boatPresenceEstimator->srf04Params.echo = echoPin;

    boatPresenceEstimator->lastValid = 0;

    if (srf04_init(&(boatPresenceEstimator->srf04), &(boatPresenceEstimator->srf04Params)) != SRF04_OK) {
        printf("ERROR SRF04 Initialization\n");
        return -1;
    }
    else
        return 1;
}

int boat_presence_estimator_get_present(boatPresenceEstimator_t *boatPresenceEstimator, int numberOfRead) {
    // The boat present estimation is based on multiple read, to avoid outliers.
    // The function return the most common value
    // This function is blockable, use numberOfRead as small as possible

    //Return 1 if the boat is present
    //Return 0 if the boat is not present
    //Return -1 if there is a read error

    int boatIsPresent = 0;
    int boatIsNotPresent = 0;

    for (int i = 0; i < numberOfRead; i++) {
        int distance_mm = srf04_get_distance(&(boatPresenceEstimator->srf04));
        bool read_error = distance_mm < 0;

        if (distance_mm > DEFAULT_BOAT_DISTANCE_THRESHOLD_MM + DEFAULT_HYSTERESIS_MM / 2) { //Distance > 1000
            boatIsNotPresent++;
        }
        else if (!read_error && distance_mm < DEFAULT_BOAT_DISTANCE_THRESHOLD_MM - DEFAULT_HYSTERESIS_MM / 2) { //Distance < 500
            boatIsPresent++;
        }
        xtimer_msleep(60);
    }

    if (boatIsPresent > boatIsNotPresent) {
        boatPresenceEstimator->lastValid = 1;
        return boatPresenceEstimator->lastValid;
    }
    else if (boatIsPresent < boatIsNotPresent) {
        boatPresenceEstimator->lastValid = 0;
        return boatPresenceEstimator->lastValid;
    }
    else {
        // if boatIsPresent == 0 and boatIsNotPresent == 0 the boat is on the deadzone, between 500 and 1000
        // Or there are read error
        // So return the last valid read
        return boatPresenceEstimator->lastValid;
    }
}