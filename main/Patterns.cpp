#include "Patterns.h"
#include "HSVTune.h"
#include <Arduino.h>

namespace PatternSelection {
    char* bitPins;
    char bits;

    unsigned int selectedPatternIndex = 0;

    /**
     * @brief Initializes the pins connected to the DIP switch being used to get the pattern index/number.
     * 
     * @param bitPins list of pins connected to the DIP switch
     * @param bits number of pins connected to the switch
     */
    void begin(char pinNumber, char* pins) {        
        char bitPins[bits = pinNumber] = {};

        for(int i = 0; i < pinNumber; i++) {
            pinMode(bitPins[i] = pins[i], INPUT_PULLUP);
        }
    }

    bool update() {
        unsigned int newPatternIndex = 0;

        for(int i = bits - 1; i >= 0; i--) {
            newPatternIndex = (newPatternIndex << 1) | digitalRead(bitPins[i]);
        }

        return newPatternIndex != (selectedPatternIndex = newPatternIndex);
    }
}

void Pattern::update(long deltaTime) {}
void Pattern::transition() {}
HSVColor Pattern::getPixel(int pixelIndex) {
    return generateColor(0, 0, 0);
}

SolidPattern::SolidPattern(HSVColor color) {
    this->setColor(color);
}

void SolidPattern::update(long deltaTime) {}

void SolidPattern::transition() {}

HSVColor SolidPattern::getPixel(int pixelIndex) {
    return this->getColor();
}

void SolidPattern::setColor(HSVColor newColor) {
    this->color = newColor;
}

HSVColor SolidPattern::getColor() {
    return this->color;
}

FirePattern::FirePattern(HSVColor litColor, HSVColor unlitColor) {
    this->litColor = litColor,
    this->unlitColor = unlitColor;
}

void FirePattern::transition() {
    this->extraEnergy = maxEnergy;
    this->flares[maxFlares] = {};
    Flare* flares = this->flares;

    for(int flareIndex = 0; flareIndex < maxFlares; flareIndex++) {
        Flare flare;

        flare.energy = 0;
        flare.position = 0;

        flares[flareIndex] = flare;
    }
}

void FirePattern::update(long deltaTime) {
    Flare* flares = this->flares;

    double energyLoss = deltaTime / flareLifespan;

    for(int flareIndex = 0; flareIndex < maxFlares; flareIndex++) {
        Flare& flare = flares[flareIndex];

        double newFlareEnergy = max(flare.energy - (double) deltaTime / flareLifespan, 0);

        this->extraEnergy += flare.energy - newFlareEnergy;

        flare.energy = newFlareEnergy;
    }

    for(int flareIndex = 0; flareIndex < maxFlares; flareIndex++) {
        Flare& flare = flares[flareIndex];

        if(0 >= flare.energy) {
            flare.energy = random(0, 100) / 100.0 * this->extraEnergy;

            this->extraEnergy -= flare.energy;

            flare.position = 0;
        }

        flare.position += flare.energy * (double) deltaTime * flareSpeed;

        // Serial.println(flare.position);
    }
}

HSVColor FirePattern::getPixel(int pixelIndex) {
    Flare* flares = this->flares;

    double lightAbundance = 0;

    for(int flareIndex = 0; flareIndex < maxFlares; flareIndex++) {
        Flare& flare = flares[flareIndex];
        // Serial.println(flare.position);

        lightAbundance += lightDiffusion / (1 + abs(pixelIndex - flare.position)) * flare.energy;
    }

    HSVColor color;

    color.hue = (this->unlitColor.hue + this->litColor.hue * lightAbundance) / (lightAbundance + 1.0);
    color.saturation = (this->unlitColor.saturation + this->litColor.saturation * lightAbundance) / (lightAbundance + 1.0);
    color.value = (this->unlitColor.value + this->litColor.value * lightAbundance) / (lightAbundance + 1.0);

    return color;
}