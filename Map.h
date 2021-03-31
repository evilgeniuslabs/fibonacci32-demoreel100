/*
   Fibonacci32 DemoReel100: https://github.com/evilgeniuslabs/fibonacci32-demoreel100
   Copyright (C) 2021 Jason Coon, Evil Genius Labs LLC

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

uint8_t physicalToFibonacci[NUM_LEDS]  = { 0, 13, 26, 31, 18, 5, 10, 23, 28, 15, 2, 7, 20, 25, 12, 4, 17, 30, 22, 9, 1, 14, 27, 19, 6, 11, 24, 29, 16, 3, 8, 21 };
uint8_t fibonacciToPhysical[NUM_LEDS]  = { 0, 20, 10, 29, 15, 5, 24, 11, 30, 19, 6, 25, 14, 1, 21, 9, 28, 16, 4, 23, 12, 31, 18, 7, 26, 13, 2, 22, 8, 27, 17, 3 };
uint8_t coordsX[NUM_LEDS]              = { 152, 224, 252, 210, 211, 184, 169, 161, 89, 121, 138, 102, 61, 13, 57, 82, 29, 0, 36, 63, 111, 79, 83, 129, 118, 160, 196, 255, 212, 163, 203, 250 };
uint8_t coordsY[NUM_LEDS]              = { 130, 110, 74, 8, 51, 97, 54, 8, 0, 29, 89, 67, 40, 90, 84, 120, 134, 166, 200, 160, 151, 210, 255, 244, 194, 214, 241, 195, 198, 168, 156, 146 };
uint8_t angles[NUM_LEDS]               = { 255, 246, 237, 214, 223, 232, 208, 199, 176, 185, 193, 170, 161, 138, 147, 132, 123, 114, 100, 108, 94, 85, 76, 62, 70, 47, 38, 15, 23, 32, 9, 0 };

void anglePalette() {
  uint8_t hues = 256 / NUM_LEDS;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = angles[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (x * hues));
  }
}

void radiusPalette() {
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t r = physicalToFibonacci[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (r * hues));
  }
}

void xPalette() {
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (x * hues));
  }
}

void yPalette() {
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t y = coordsY[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (y * hues));
  }
}

void xyPalette() {
  uint8_t hues = 1;

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];
    uint8_t y = coordsY[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - ((x + y) * hues));
  }
}

void angleGradientPalette() {
 uint8_t hues = 1;

 for (uint8_t i = 0; i < NUM_LEDS; i++) {
   uint8_t x = angles[i];

   leds[i] = ColorFromPalette(currentGradientPalette, beat8(speed) - (x * hues));
 }
}

void radiusGradientPalette() {
 uint8_t hues = 1;

 for (uint8_t i = 0; i < NUM_LEDS; i++) {
   uint8_t r = physicalToFibonacci[i];

   leds[i] = ColorFromPalette(currentGradientPalette, beat8(speed) - (r * hues));
 }
}

void xGradientPalette() {
 uint8_t hues = 1;

 for (uint8_t i = 0; i < NUM_LEDS; i++) {
   uint8_t x = coordsX[i];

   leds[i] = ColorFromPalette(currentGradientPalette, beat8(speed) - (x * hues));
 }
}

void yGradientPalette() {
 uint8_t hues = 1;

 for (uint8_t i = 0; i < NUM_LEDS; i++) {
   uint8_t y = coordsY[i];

   leds[i] = ColorFromPalette(currentGradientPalette, beat8(speed) - (y * hues));
 }
}

void xyGradientPalette() {
 uint8_t hues = 1;

 for (uint8_t i = 0; i < NUM_LEDS; i++) {
   uint8_t x = coordsX[i];
   uint8_t y = coordsY[i];

   leds[i] = ColorFromPalette(currentGradientPalette, beat8(speed) - ((x + y) * hues));
 }
}
