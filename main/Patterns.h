#include <Adafruit_NeoPixel.h>
#include "HSVTune.h"

#ifndef PATTERNS_H
#define PATTERNS_H

class Pattern {
    public:
        virtual void update(long deltaTime);
        // virtual void transition();
        virtual void transitionIn();
        virtual void transitionOut();
        virtual void getPixel(int pixelIndex, HSVColor* result);
};

class SolidPattern : public Pattern {
    public:
        SolidPattern(HSVColor color);

        void update(long deltaTime) override;

        // void transition() override;
        void transitionIn() override;
        void transitionOut() override;

        void getPixel(int pixelIndex, HSVColor* result) override;

        void setColor(HSVColor color);

        HSVColor getColor();

    private:
        HSVColor color;
};


#define MAX_FLARES 5
#define MAX_ENERGY 2.5
#define FLARE_LIFESPAN 5
#define FLARE_SPEED 0.1
#define LIGHT_DIFFUSION 0.5

class FirePattern : public Pattern {
    public:
        FirePattern(HSVColor litColor, HSVColor unlitColor, int ledCount);

        // void transition() override;
        void transitionIn() override;
        void transitionOut() override;

        void update(long deltaTime) override;

        void getPixel(int pixelIndex, HSVColor* result) override;

    private:
        struct Flare {
            double energy;
            double position;
        };
        HSVColor unlitColor;
        HSVColor litColor;
        int ledCount;
        double extraEnergy;

        double totalEnergy();

        Flare *getFlares();

        Flare flares[MAX_FLARES];
};

#define PROBE_DIFFUSION 1
#define PROBE_SPEED 0.02
class ProbePattern : public Pattern {
    public:
        ProbePattern(HSVColor color, int ledCount);

        void update(long deltaTime) override;

        // void transition() override;
        void transitionIn() override;
        void transitionOut() override;

        void getPixel(int pixelIndex, HSVColor* result) override;

    private:
        HSVColor color;
        int ledCount;
};
#endif