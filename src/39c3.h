#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "NameTag.h"

constexpr static const uint8_t PROGMEM _NeoPixelGammaTableC[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
    3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  7,
    7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
   13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
   20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
   30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42,
   42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
   58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75,
   76, 77, 78, 80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96,
   97, 99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,
  122,124,125,127,129,130,132,134,136,137,139,141,143,145,146,148,
  150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,
  182,184,186,188,191,193,195,197,199,202,204,206,209,211,213,215,
  218,220,223,225,227,230,232,235,237,240,242,245,247,250,252,255};

  
constexpr static uint8_t gamma8(uint8_t x) {
    return _NeoPixelGammaTableC[x];
}
    
constexpr uint32_t gamma32(uint32_t x) {
    // Extract individual bytes, apply gamma correction, and reassemble
    uint8_t b0 = gamma8((x >>  0) & 0xFF);
    uint8_t b1 = gamma8((x >>  8) & 0xFF);
    uint8_t b2 = gamma8((x >> 16) & 0xFF);
    uint8_t b3 = gamma8((x >> 24) & 0xFF);
    
    return ((uint32_t)b3 << 24) | ((uint32_t)b2 << 16) | ((uint32_t)b1 << 8) | b0;
}
    
static constexpr uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}
constexpr uint32_t panelNeonGreen = gamma32(Color(0x20, 0xFF, 0x00));
constexpr uint32_t panelElectricViolet = gamma32(Color(0xA0, 0x73, 0xdd));

void update39c3(unsigned long time, uint8_t brightness) {
    using namespace NameTag;
    static uint16_t mask = 0x1FF;
    static uint8_t counter = 100;

    brightness = brightness >> 4;

    uint32_t neonGreen = tinyNeoPixel::Color(0, brightness, 0);
    uint32_t electricViolet = tinyNeoPixel::Color(scale8(0x93, brightness), scale8(0x33, brightness), scale8(0xdd, brightness));
    
    if (--counter == 0) {
        uint8_t bit = random8(9);
        mask ^= (1 << bit);
        counter = random8(30, 255);
    }
    
    for (int i =0; i < 8; i++) {
        leds.setPixelColor(i, mask & (1 << i) ? neonGreen : electricViolet);
    }
    
    if (mask & (1 << 8)) {
        setPanelColor(panelNeonGreen, brightness<<2); // neon green
    } else {
        setPanelColor(panelElectricViolet, brightness<<2); // electric violet
    }

    if (counter <= 10) {
        leds.setPixelColor(5, 0);
    }
    leds.show();
};
