#ifndef IOTGROUPPROJECT_DOCK_H
#define IOTGROUPPROJECT_DOCK_H

#include "srf04.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "stdbool.h"

#define DEFAULT_HYSTERESIS_MM 500
#define DEFAULT_BOAT_DISTANCE_THRESHOLD_MM 750

typedef struct dockSetting_s {
    uint16_t dockId;
    gpio_t switchPin;
    gpio_t ledPin;
}dockSetting_t;

typedef struct dock_s {
    dockSetting_t * setting;
    bool lastRead;
    uint8_t readCounter;
    bool ledState;
}dock_t;

void dock_init(dock_t *dock, dockSetting_t *setting);
void dock_toggle_led(dock_t * dock, bool state);
int dock_get_present(dock_t *dock);
uint16_t dock_get_id(dock_t * dock);
bool dock_set_last_read(dock_t *dock, bool lastRead);
bool dock_get_last_read(dock_t * dock);
bool dock_get_led_state(dock_t *dock);
bool dock_is_first_read(dock_t *dock);
#endif //IOTGROUPPROJECT_DOCK_H
