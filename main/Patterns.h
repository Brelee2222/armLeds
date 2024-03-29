#include <Adafruit_NeoPixel.h>
#include "HSVTune.h"

#ifndef PATTERNS_H
#define PATTERNS_H

namespace PatternSelection {
    extern unsigned int selectedPatternIndex;

    /**
     * @brief Initializes the pins connected to the DIP switch being used to get the pattern index/number.
     * 
     * @param bitPins list of pins connected to the DIP switch
     * @param bits number of pins connected to the switch
     */
    void begin(char bits, char* bitPins);

    bool update();
}

class Pattern {
    public:
        virtual void update(long deltaTime);
        virtual void transition();
        virtual HSVColor getPixel(int pixelIndex);
};

class SolidPattern : public Pattern {
    public:

        void update(long deltaTime) override;

        void transition() override;

        HSVColor getPixel(int pixelIndex) override;

        SolidPattern(HSVColor color);

        void setColor(HSVColor color);

        HSVColor getColor();

    private:
        HSVColor color;
};

#define maxFlares 5
#define maxEnergy 5
#define flareLifespan 5
#define flareSpeed 0.05
#define lightDiffusion 0.5

class FirePattern : public Pattern {
    public:
        FirePattern(HSVColor litColor, HSVColor unlitColor);

        void transition() override;

        void update(long deltaTime) override;

        HSVColor getPixel(int pixelIndex) override;

    private:
        HSVColor unlitColor;
        HSVColor litColor;

        double extraEnergy;

        struct Flare {
            double energy;
            double position;
        };

        Flare flares[maxFlares];

        double totalEnergy();
};
#endif