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

#include <FastLED.h>     // https://github.com/FastLED/FastLED
#include <ClickButton.h> // https://github.com/marcobrianza/ClickButton

FASTLED_USING_NAMESPACE

// Based on FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    11
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    32

CRGB leds[NUM_LEDS];

#define FRAMES_PER_SECOND 120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t hue = 0; // rotating "base color" used by many of the patterns
uint8_t speed = 30;

#include "Palettes.h"
#include "GradientPalettes.h"
#include "Map.h"
#include "ColorWaves.h"
#include "Pride.h"

ClickButton buttonPattern   (6, LOW, CLICKBTN_PULLUP); // Connect your button between pin 6 and GND
ClickButton buttonBrightness(7, LOW, CLICKBTN_PULLUP); // Connect your button between pin 7 and GND

boolean autoplay = false;
uint8_t autoplaySeconds = 10;

boolean power = true;

boolean autoplayPalettes = true;
uint8_t autoplayPaletteSeconds = 10;

uint8_t brightnesses[] = {8, 16, 32, 64, 128, 255};
uint8_t brightnessCount = ARRAY_SIZE(brightnesses);
uint8_t brightnessIndex = 2;

void setup() {
  //  delay(3000); // 3 second delay for recovery

  Serial.begin(9600);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setCorrection(TypicalSMD5050);
  // FastLED.setDither(0);

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

  FastLED.setBrightness(brightnesses[brightnessIndex]);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList patterns = {
  colorWavesFibonacci,
  colorWaves,

  prideFibonacci,
  pride,

  fireFibonacci,
  waterFibonacci,

  anglePalette,
  radiusPalette,
  xPalette,
  yPalette,
  xyPalette,

  angleGradientPalette,
  radiusGradientPalette,
  xGradientPalette,
  yGradientPalette,
  xyGradientPalette,

  rainbow,
  rainbowWithGlitter,
  confetti,
  sinelon,
  juggle,
  bpm
};

const uint8_t patternCount = ARRAY_SIZE(patterns);

uint8_t currentPatternIndex = 0; // Index number of which pattern is current

void loop()
{
  // update the buttons
  buttonPattern.Update();
  buttonBrightness.Update();

  // check the buttons

  // check the pattern button
  if (buttonPattern.changed) {
    // pattern button long clicked?
    if (buttonPattern.clicks < 0) {
      Serial.println("Button 6 was long clicked, autoplay on");
      autoplayOn();
    }
    else if (buttonPattern.clicks > 0) {
      Serial.println("Button 6 was clicked");

      // if power is off, any button press should turn it back on without changing anything else
      if (!power) {
        powerOn();
      } else if (autoplay) { // if autoplay is on, just stop on the current pattern
        autoplayOff();
      } else { // otherwise, advance to the next pattern
        nextPattern();
      }
    }
  }

  // check the brightness/power button
  if (buttonBrightness.changed) {
    // brightness/power button long clicked?
    if (buttonBrightness.clicks < 0) {
      Serial.println("Button 7 was long pressed, power off");
      powerOff();
    }
    else if (buttonBrightness.clicks > 0) {
      Serial.println("Button 7 was clicked, next brightness");
      powerOn();
      nextBrightness();
    }
  }

  if (!power) {
    FastLED.clear();
    FastLED.show();
    FastLED.delay(1000 / FRAMES_PER_SECOND);
    return;
  }

  // Call the current pattern function once, updating the 'leds' array
  patterns[currentPatternIndex]();

  // do some periodic updates
  
  EVERY_N_MILLISECONDS(20) {
    hue++; // slowly cycle the "base color" through the rainbow
  }

  EVERY_N_SECONDS(autoplaySeconds) {
    if (autoplay) {
      nextPattern(); // change patterns periodically
    }
  }

  EVERY_N_SECONDS(autoplayPaletteSeconds) {
    // change palettes periodically
    if (autoplayPalettes) {
      nextPalette();
      nextGradientPalette();
    }
  }

  EVERY_N_MILLISECONDS(40) {
    nblendPaletteTowardPalette(currentGradientPalette, targetGradientPalette, 8); // slowly blend the current palette to the next
  }

  // send the 'leds' array out to the actual LED strip
  FastLED.show();

  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void autoplayOn() {
  if (!autoplay) {
    Serial.println("autoplay on");
    autoplay = true;
  }
}

void autoplayOff() {
  if (autoplay) {
    Serial.println("autoplay off");
    autoplay = false;
  }
}

void powerOn() {
  if (!power) {
    Serial.println("power on");
    power = true;
  }
}

void powerOff() {
  if (power) {
    Serial.println("power off");
    power = false;
  }
}

void nextGradientPalette()
{
  Serial.println("nextGradientPalette");
  // add one to the current gradient palette number, and wrap around at the end
  currentGradientPaletteIndex = (currentGradientPaletteIndex + 1) % gradientPaletteCount;

  targetGradientPalette = gradientPalettes[currentGradientPaletteIndex];
}

void nextPalette()
{
  Serial.println("nextPalette");
  // add one to the current palette number, and wrap around at the end
  currentPaletteIndex = (currentPaletteIndex + 1) % paletteCount;
}

void nextPattern()
{
  Serial.println("nextPattern");
  // add one to the current pattern number, and wrap around at the end
  currentPatternIndex = (currentPatternIndex + 1) % patternCount;
}

void nextBrightness() {
  brightnessIndex = (brightnessIndex + 1) % brightnessCount;
  uint8_t brightness = brightnesses[brightnessIndex];
  FastLED.setBrightness(brightness);
  
  Serial.print("setting brightness to ");
  Serial.println(brightness);
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, hue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter(fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(hue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(hue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void fireFibonacci() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];
    uint8_t y = coordsY[i];

    uint8_t n = qsub8(inoise8((x << 2) - beat88(speed << 2), (y << 2)), x);

    leds[i] = ColorFromPalette(HeatColors_p, n);
  }
}

void waterFibonacci() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];
    uint8_t y = coordsY[i];

    uint8_t n = inoise8((x << 2) + beat88(speed << 2), (y << 4));

    leds[i] = ColorFromPalette(IceColors_p, n);
  }
}
