#include "Patterns.h"
#include "HSVTune.h"
#include <Arduino.h>

//TODO: Clean up code and make a bit more efficient.

void Pattern::setLEDCount(int ledCount) {
    Pattern::ledCount = ledCount;
}
int Pattern::getLEDCount() {
    return Pattern::ledCount;
}
void Pattern::update() {}
void Pattern::transitionIn() {}
void Pattern::transitionOut() {}
void Pattern::getPixel(int pixelIndex, HSVColor* result) {}
int Pattern::ledCount;

SolidPattern::SolidPattern(HSVColor color) {
    this->setColor(color);
}

void SolidPattern::getPixel(int pixelIndex, HSVColor* result) {
    *result = this->getColor();
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

void FirePattern::transitionIn() {
    this->lastTime = millis();
    this->extraEnergy = MAX_ENERGY;
    this->flares[MAX_FLARES] = {};

    Flare* flares = this->flares;

    for(int flareIndex = 0; flareIndex < MAX_FLARES; flareIndex++) {
        // this->flares[flareIndex] = Flare();

        Flare& flares = this->flares[flareIndex];

        flares.energy = flares.position = 0;
    }
}
// void FirePattern::transitionOut() {
//     // free(this->flares);
//     // free(&this->extraEnergy);
//     // free(&this->lastTime);
// }

void FirePattern::update() {
    unsigned long deltaTime = millis() - this->lastTime;
    // this->lastTime += deltaTime;
    this->lastTime = millis(); // It's possible this takes more processing and drains more power

    Flare* flares = this->flares;

    double energyLoss = deltaTime / FLARE_LIFESPAN; // Energy loss is the same for all flares.

    for(int flareIndex = 0; flareIndex < MAX_FLARES; flareIndex++) { // Calculates the new amount of extra energy.
        Flare& flare = flares[flareIndex];

        double newFlareEnergy = max(flare.energy - (double) deltaTime / FLARE_LIFESPAN, 0);

        this->extraEnergy += flare.energy - newFlareEnergy;

        flare.energy = newFlareEnergy;
    }

    for(int flareIndex = 0; flareIndex < MAX_FLARES; flareIndex++) { // Gives a random amount of the extra energy to flares without energy, and assigns those flares a position of 0 (the starting position of a flare).
        Flare& flare = flares[flareIndex];

        if(0 >= flare.energy) {
            flare.energy = random(0, 100) / 100.0 * this->extraEnergy;

            this->extraEnergy -= flare.energy;

            flare.position = 0;
        }

        flare.position += flare.energy * (double) deltaTime * FLARE_SPEED;
    }

    this->extraEnergy = max(min(this->extraEnergy, MAX_ENERGY), 0.0);
}

void FirePattern::getPixel(int pixelIndex, HSVColor* result) {
    int ledCount = Pattern::getLEDCount();

    Flare* flares = this->flares;

    pixelIndex = ledCount / 2 - abs(pixelIndex - ledCount / 2); // the pixelIndex is modified so that the fire pattern is mirrored.

    double lightAbundance = 0; // light abundance is how much light a pixel is receiving.

    for(int flareIndex = 0; flareIndex < MAX_FLARES; flareIndex++) {
        Flare& flare = flares[flareIndex];

        lightAbundance += LIGHT_DIFFUSION / (1 + abs((double) pixelIndex - flare.position)) * flare.energy; 
    }

    // The calculations are basically a calculation of weighted average.
    result->hue = (this->unlitColor.hue + this->litColor.hue * lightAbundance) / (lightAbundance + 1.0);
    result->saturation = (this->unlitColor.saturation + this->litColor.saturation * lightAbundance) / (lightAbundance + 1.0);
    result->value = (this->unlitColor.value + this->litColor.value * lightAbundance) / (lightAbundance + 1.0);
}

ProbePattern::ProbePattern(HSVColor color) {
    this->color = color;
}
void ProbePattern::getPixel(int pixelIndex, HSVColor* result) {
    int ledCount = Pattern::getLEDCount();

    int position = (unsigned int) (millis() * PROBE_SPEED) % ledCount; // There is a cast to uint because floating points cannot be modded 3:
    
    double abundance = 1.0 + min(abs(pixelIndex - position), abs(abs(pixelIndex - position) - ledCount)) / LIGHT_DIFFUSION;

    result->hue = this->color.hue;
    result->saturation = this->color.saturation;
    result->value = this->color.value / abundance;
}

void RainbowPattern::getPixel(int pixelIndex, HSVColor* result) {
    int ledCount = Pattern::getLEDCount();

    // int color = (unsigned int) (millis() * PROBE_SPEED) % ledCount; // I can't divide by led count then mod by 1 because doubles cannot be modded.

    result->hue = (pixelIndex + (unsigned int) (millis() * PROBE_SPEED) % ledCount) / (double) ledCount * 65535;
    result->saturation = 255;
    result->value = 255;
}

BlinkPattern::BlinkPattern(HSVColor color) {
    this->color = color;
}
void BlinkPattern::getPixel(int pixelIndex, HSVColor* result) {

    if(((int) (millis() / BLINK_FREQUENCY) ^ (pixelIndex)) & 1) { // This calculation uses a heavy amount of bitwises; it is basically divides milliseconds by frequency, adds the pixel index then sees if the result is divisible by 2.
        result->hue = this->color.hue;
        result->saturation = this->color.saturation;
        result->value = this->color.value;
        return;
    }

    result->hue = result->saturation = result->value = 0;
}