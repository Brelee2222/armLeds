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
        virtual void transitionIn();
        virtual void transitionOut();
        virtual void getPixel(int pixelIndex, HSVColor* color);
};

class SolidPattern : public Pattern {
    public:

        void update(long deltaTime) override;

        void transitionIn() override;
        void transitionOut() override;

        void getPixel(int pixelIndex, HSVColor* color) override;

        SolidPattern(HSVColor color);

        void setColor(HSVColor color);

        HSVColor getColor();

    private:
        HSVColor color;
};

#define maxFlares 10
#define maxEnergy 20
#define flareLifespan 6.0
#define flareSpeed 0.01
#define lightDiffusion 0.2

class FirePattern : public Pattern {
    public:
        FirePattern(HSVColor litColor, HSVColor unlitColor, int pixelNumber);

        void transitionIn() override;
        void transitionOut() override;

        void update(long deltaTime) override;

        void getPixel(int pixelIndex, HSVColor* color) override;

    private:
        HSVColor unlitColor;
        HSVColor litColor;

        double extraEnergy;

        struct Flare {
            double energy;
            double position;
        };

        Flare flares[maxFlares];

        double getExtraEnergy();
        void setExtraEnergy(double energy);

        Flare* getFlares();

        unsigned int pixelNum;
};
#endif