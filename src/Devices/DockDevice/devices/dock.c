#include "dock.h"
#include "stdint.h"
#include "../../Common/conversion.h"
#include "random.h"

#define FAKE_SENSOR_UPDATE_INTERVAL_S 60*3
#ifdef FAKE_SENSOR
uint32_t lastUpdate;
#endif
#define NUMBER_OF_READ 5        //Use an odd value
#define MEASUREMENT_INTERVAL_MS 50

void dock_init(dock_t *dock, dockSetting_t *setting) {
#ifdef FAKE_SENSOR
    random_init(xtimer_now_usec());
    lastUpdate = 0;
    printf("Fake sensor initialized\n");
    (void)setting;
#else
    dock->setting = setting;
    gpio_init(setting->switchPin, GPIO_IN_PU);
    gpio_init(setting->ledPin, GPIO_OUT);
#endif
    dock->readCounter = 0;
}

void dock_toggle_led(dock_t *dock, bool state) {
    const dockSetting_t *setting = dock->setting;
    gpio_write(setting->ledPin, state);
    dock->ledState = state;
}

int dock_get_present(dock_t *dock) {
    //Return 1 if the boat is present
    //Return 0 if the boat is not present
#ifdef FAKE_SENSOR
    if(US2S(xtimer_now_usec() - lastUpdate) > FAKE_SENSOR_UPDATE_INTERVAL_S){
        //Fake sensor is updated every 3 minutes
        dock->lastRead = random_uint32_range(0,10) > 5;
    }
    return dock->lastRead;
#else
    // It get NUMBER_OF_READ measurements, one every MEASUREMENT_INTERVAL_MS, the most frequent is returned
    uint8_t trueValue = 0;
    uint8_t falseValue = 0;
    const dockSetting_t *setting = dock->setting;
    for (uint8_t i = 0; i < NUMBER_OF_READ; i++) {
        if (gpio_read(setting->switchPin) > 0)
            trueValue++;
        else
            falseValue++;
        xtimer_usleep(MS2US(MEASUREMENT_INTERVAL_MS));
    }
    dock->readCounter++;
    return trueValue > falseValue;
#endif
}

uint16_t dock_get_id(dock_t *dock) {
    return dock->setting->dockId;
}

bool dock_get_led_state(dock_t *dock) {
    return dock->ledState;
}

bool dock_set_last_read(dock_t *dock, bool lastRead) {
    return dock->lastRead = lastRead;
}

bool dock_get_last_read(dock_t *dock) {
    return dock->lastRead;
}

bool dock_is_first_read(dock_t *dock) {
    return dock->readCounter == 1;
}

