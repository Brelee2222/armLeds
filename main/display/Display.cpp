#include "Display.h"

#include "Menu.h"
#include "Pattern.h"

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastTime = 0;

void update() {
    unsigned long time = millis();

    currentPattern->update(time);

    currentMenu->update();

    lastTime = time;
}

void display() {
    HSVColor pixelColor = HSVColor();

    for(unsigned int pixelIndex = 0; pixelIndex < LED_COUNT; pixelIndex++) {
        currentPattern->getPixel(pixelIndex, &pixelColor);

        leds.setPixelColor(pixelIndex, leds.ColorHSV(pixelColor.hue, pixelColor.saturation, pixelColor.value));
    }

    currentMenu->display(&leds);
}