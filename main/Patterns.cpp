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
void Pattern::transitionIn() {}
void Pattern::transitionOut() {}

void Pattern::getPixel(int pixelIndex, HSVColor* color) {}

SolidPattern::SolidPattern(HSVColor color) {
    this->setColor(color);
}

void SolidPattern::update(long deltaTime) {}

void SolidPattern::transitionIn() {}
void SolidPattern::transitionOut() {}

void SolidPattern::getPixel(int pixelIndex, HSVColor* color) {
    color->hue = this->color.hue;
    color->saturation = this->color.saturation;
    color->value = this->color.value;
}

void SolidPattern::setColor(HSVColor newColor) {
    this->color = newColor;
}

HSVColor SolidPattern::getColor() {
    return this->color;
}


FirePattern::FirePattern(HSVColor litColor, HSVColor unlitColor, int pixelNumber) {
    this->litColor = litColor,
    this->unlitColor = unlitColor;
    this->extraEnergy = maxEnergy;
    this->pixelNum = pixelNumber;
}

void FirePattern::transitionIn() {
    this->flares[maxFlares] = {};
}

void FirePattern::transitionOut() {
    delete this->flares;
}

void FirePattern::update(long deltaTime) {
    Flare* flares = this->getFlares();

    double 
        energyLoss = (double) deltaTime / flareLifespan,
        extraEnergy = this->getExtraEnergy(),
        deltaPosition = (double) deltaTime * flareSpeed,
        newFlareEnergy
    ;


    for(int flareIndex = 0; flareIndex < maxFlares; flareIndex++) {
        Flare& flare = flares[flareIndex];

        newFlareEnergy = max(flare.energy - energyLoss, 0);

        extraEnergy += flare.energy - newFlareEnergy;

        flare.energy = newFlareEnergy;

        if(0 >= flare.energy) {
            flare.energy = random(0, 100) / 100.0 * extraEnergy;

            extraEnergy -= flare.energy;

            flare.position = 0;
        }

        flare.position += flare.energy * deltaPosition;
    }

    this->setExtraEnergy(extraEnergy);
}

void FirePattern::getPixel(int pixelIndex, HSVColor* color) {
    Flare* flares = this->getFlares();

    double lightAbundance = 0;

    for(int flareIndex = 0; flareIndex < maxFlares; flareIndex++) {
        Flare& flare = flares[flareIndex];

        double position = flare.position;

        if(flareIndex & 1) {
            position = this->pixelNum - position;
        }
        // Serial.println(flare.position);

        lightAbundance += lightDiffusion / (1 + abs(pixelIndex - position)) * flare.energy;
    }

    double divisor = (lightAbundance + 1.0);

    color->hue = (this->unlitColor.hue + this->litColor.hue * lightAbundance) / divisor;
    color->saturation = (this->unlitColor.saturation + this->litColor.saturation * lightAbundance) / divisor;
    color->value = (this->unlitColor.value + this->litColor.value * lightAbundance) / divisor;
}

FirePattern::Flare* FirePattern::getFlares() {
    return this->flares;
}

double FirePattern::getExtraEnergy() {
    return this->extraEnergy;
}

void FirePattern::setExtraEnergy(double energy) {
    this->extraEnergy = energy;
}