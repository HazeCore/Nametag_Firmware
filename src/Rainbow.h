#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "NameTag.h"

// 34, 41, 50, 50, 45, 41, 3, 3

#define pos(D) D // position modifications could be performed here
const uint8_t ledPositions[NameTag::LED_COUNT] PROGMEM = { 34, 41, 50, 50, 45, 41, 3, 3 };

void updateRainbow(unsigned long time, uint8_t brightness) {
    using namespace NameTag;

    brightness = min(brightness+10, 100);

    uint16_t p = time << 3;

    for (uint8_t i = 0; i < LED_COUNT; i++) {
        leds.setPixelColor(i, gammaHSV(uint16_t(pgm_read_byte(&ledPositions[i]))*256 + p, brightness));
    }

    brightness += 10;

    setPanelColor(gammaHSV(pos(20)*256 + p, brightness));

    leds.show();
};