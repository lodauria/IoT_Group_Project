#ifndef IOTGROUPPROJECT_SSD1306_H
#define IOTGROUPPROJECT_SSD1306_H

#include "u8g2.h"

typedef struct ssd1306_s {
    uint8_t i2cAddr;
    u8g2_t u8g2;
} ssd1306_t;

typedef enum arrowDirection {
    LEFT,
    RIGHT,
    TOP,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
}arrowDirection_e;

void ssd1306_init(ssd1306_t *ssd1306, uint8_t i2cAddr);

void ssd1306_writeArrow(ssd1306_t *ssd1306, arrowDirection_e direction);

#endif //IOTGROUPPROJECT_SSD1306_H
