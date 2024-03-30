#include "HSVTune.h"
#include <Arduino.h>

struct HSVColor;

HSVColor generateColor(short hue, char saturation, char value) {
    HSVColor color;

    color.hue = hue,
    color.saturation = saturation,
    color.value = value;

    return color;
}

namespace HSVTransform {
    void transformColor(HSVColor *color, HSVColor transform) {
        
        // Serial.println(transform.hue);

        color->hue += transform.hue;
        // newColor.saturation = sqrt(color.saturation * transform.saturation);
        // newColor.saturation = min(color.saturation, transform.saturation);
        color->saturation *= transform.saturation / 255;
        // newColor.value = sqrt(color.value * transform.value);
        // newColor.value = min(color.value, transform.value);
        color->value *= transform.value / 255;
    }
}

namespace HSVTune {

    char huePin;
    char satPin;
    char valPin;

    short hueOffset = 0;

    void begin(char hue_pin, char saturation_pin, char value_pin) {
        pinMode(huePin = hue_pin, INPUT_PULLUP);
        pinMode(satPin = saturation_pin, INPUT_PULLUP);
        pinMode(valPin = value_pin, INPUT_PULLUP);

        if(USE_HUE_OFFSET)
            hueOffset = ((double) (analogRead(hue_pin) - 7) / 200 * 255) * (1 << 8);
    }

    double __halfLife = 0;

    void setHalfLife(double millis) {
        __halfLife = millis;
    }
    
    double getHalfLife() {
        return __halfLife;
    }


    float hue = 0;
    float sat = 0;
    float val = 0;

    long previousTime = 0;

    void update() {
        double deltaTime = millis() - previousTime;
        previousTime = millis();

        double divisor = __halfLife + deltaTime;

        hue = (hue * __halfLife + (analogRead(huePin) - 7) * deltaTime) / divisor;
        sat = (sat * __halfLife + (analogRead(satPin) - 7) * deltaTime) / divisor;
        val = (val * __halfLife + (analogRead(valPin) - 7) * deltaTime) / divisor;
    }

    unsigned short getHueModifier() {
        return ((double) hue / 200 * 255) * (1 << 8) - hueOffset;
    }

    unsigned char getSaturationModifier() {
        return min(255 - (sat - 13) / 199 * 255, 255);
    };
    unsigned char getValueModifier() {
        return min(255 - (val - 13) / 199 * 255, 255);
    }

    HSVColor getColorModifier() {
        HSVColor modColor;
        
        modColor.hue = getHueModifier();
        modColor.saturation = getSaturationModifier();
        modColor.value = getValueModifier();

        return modColor;
    }
}