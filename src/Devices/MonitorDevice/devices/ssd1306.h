#ifndef IOTGROUPPROJECT_SSD1306_H
#define IOTGROUPPROJECT_SSD1306_H

#include "u8g2.h"
#include "stdint.h"

#define MAX_QUEUE_ELEMENTS 16
#define MAX_PLATE_TEXT 64

typedef enum arrowDirection {
    LEFT,
    RIGHT,
    TOP,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    TURN_AROUND,
    OFF
} arrowDirection_e;

typedef struct directionArrayElement_s {
    arrowDirection_e arrowDirection;
    char text[MAX_PLATE_TEXT];
    uint8_t currentStartPosition;
    uint8_t enabled;
} directionArrayElement_t;

typedef struct ssd1306_s {
    uint8_t i2cAddr;
    u8g2_t u8g2;
    directionArrayElement_t directionArray[MAX_QUEUE_ELEMENTS];
} ssd1306_t;

void ssd1306_init(ssd1306_t *ssd1306, uint8_t i2cAddr);

void ssd1306_addIndication(ssd1306_t *ssd1306, arrowDirection_e direction, char *plate);
void ssd1306_removeIndication(ssd1306_t *ssd1306, char *plate);
#endif //IOTGROUPPROJECT_SSD1306_H
