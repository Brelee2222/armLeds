#include "HSVTune.h"
#include <Arduino.h>

struct HSVColor;

HSVColor::HSVColor(
    unsigned short hue,
    unsigned char saturation,
    unsigned char value
) {
    this->hue = hue;
    this->saturation = saturation;
    this->value = value;
};

HSVColor::HSVColor() {};

namespace HSVTransform {
    void transformColor(HSVColor *color, HSVColor transform) {
        
        // Serial.println(transform.hue);

        color->hue += transform.hue;
        // newColor.saturation = sqrt(color.saturation * transform.saturation);
        // newColor.saturation = min(color.saturation, transform.saturation);
        color->saturation *= transform.saturation / 255.0;
        // newColor.value = sqrt(color.value * transform.value);
        // newColor.value = min(color.value, transform.value);
        color->value *= transform.value / 255.0;
    }
}

namespace HSVTune {
    short hueOffset = 0;
    float hue = 0;
    float sat = 0;
    float val = 0;

    void begin() {
        pinMode(HUE_PIN, INPUT_PULLUP);
        pinMode(SAT_PIN, INPUT_PULLUP);
        pinMode(VAL_PIN, INPUT_PULLUP);

        if(USE_HUE_OFFSET)
            hueOffset = ((double) (analogRead(HUE_PIN) - 7) / 200.0 * 255) * (1 << 8);
    }

    long previousTime = 0;

    void update() {
        double deltaTime = millis() - previousTime;
        previousTime = millis();

        double divisor = TRANSITION_HALFLIFE + deltaTime;

        hue = (hue * TRANSITION_HALFLIFE + (analogRead(HUE_PIN) - 7) * deltaTime) / divisor;
        sat = (sat * TRANSITION_HALFLIFE + (analogRead(SAT_PIN) - 7) * deltaTime) / divisor;
        val = (val * TRANSITION_HALFLIFE + (analogRead(VAL_PIN) - 7) * deltaTime) / divisor;
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
        return HSVColor(
            getHueModifier(), 
            getSaturationModifier(), 
            getValueModifier()
        );
    }
}