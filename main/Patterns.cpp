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
void Pattern::getPixel(int pixelIndex, HSVColor* result) {}

SolidPattern::SolidPattern(HSVColor color) {
    this->setColor(color);
}

void SolidPattern::update(long deltaTime) {}

void SolidPattern::transition() {}

void SolidPattern::getPixel(int pixelIndex, HSVColor* result) {
    *result = this->getColor();
}

void SolidPattern::setColor(HSVColor newColor) {
    this->color = newColor;
}

HSVColor SolidPattern::getColor() {
    return this->color;
}

FirePattern::FirePattern(HSVColor litColor, HSVColor unlitColor, int ledCount) {
    this->litColor = litColor,
    this->unlitColor = unlitColor;
    this->ledCount = ledCount;
}

void FirePattern::transition() {
    this->extraEnergy = MAX_ENERGY;
    this->flares[MAX_FLARES] = {};

    Flare* flares = this->flares;

    for(int flareIndex = 0; flareIndex < MAX_FLARES; flareIndex++) {
        Flare& flare = flares[flareIndex];
        // Flare flare;

        flare.energy = 0;
        flare.position = 0;
        
        // flares[flareIndex] = flare;
    }
}

FirePattern::Flare* FirePattern::getFlares() {
    return this->flares;
};

void FirePattern::update(long deltaTime) {
    Flare* flares = this->getFlares();

    double energyLoss = deltaTime / FLARE_LIFESPAN;

    for(int flareIndex = 0; flareIndex < MAX_FLARES; flareIndex++) {
        Flare& flare = flares[flareIndex];

        double newFlareEnergy = max(flare.energy - (double) deltaTime / FLARE_LIFESPAN, 0);

        this->extraEnergy += flare.energy - newFlareEnergy;

        flare.energy = newFlareEnergy;
    }

    for(int flareIndex = 0; flareIndex < MAX_FLARES; flareIndex++) {
        Flare& flare = flares[flareIndex];

        if(0 >= flare.energy) {
            flare.energy = random(0, 100) / 100.0 * this->extraEnergy;

            this->extraEnergy -= flare.energy;

            flare.position = 0;
        }

        flare.position += flare.energy * (double) deltaTime * FLARE_SPEED;

        // Serial.println(flare.position);
    }

    this->extraEnergy = max(min(this->extraEnergy, MAX_ENERGY), 0.0);
}

void FirePattern::getPixel(int pixelIndex, HSVColor* result) {

    Flare* flares = this->getFlares();

    pixelIndex = (this->ledCount / 2) - abs(pixelIndex - (this->ledCount / 2));

    double lightAbundance = 0;

    for(int flareIndex = 0; flareIndex < MAX_FLARES; flareIndex++) {
        Flare& flare = flares[flareIndex];

        // Serial.println(flare.position);

        lightAbundance += LIGHT_DIFFUSION / (1 + abs((double) pixelIndex - flare.position)) * flare.energy;
    }

    result->hue = (this->unlitColor.hue + this->litColor.hue * lightAbundance) / (lightAbundance + 1.0);
    result->saturation = (this->unlitColor.saturation + this->litColor.saturation * lightAbundance) / (lightAbundance + 1.0);
    result->value = (this->unlitColor.value + this->litColor.value * lightAbundance) / (lightAbundance + 1.0);
}

ProbePattern::ProbePattern(HSVColor color, int ledCount) {
    this->color = color;
    this->ledCount = ledCount;
}

void ProbePattern::update(long deltaTime) {}

void ProbePattern::transition() {}

void ProbePattern::getPixel(int pixelIndex, HSVColor* result) {
    int position = (unsigned int) (millis() * PROBE_SPEED) % this->ledCount;
    double abundance = (double) 1.0 + min(abs(pixelIndex - position), abs(abs(pixelIndex - position) - this->ledCount)) / LIGHT_DIFFUSION;

    result->hue = this->color.hue;
    result->saturation = this->color.saturation;
    result->value = this->color.value / abundance;
}