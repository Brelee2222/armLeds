#include <Adafruit_NeoPixel.h>
#include "HSVTune.h"

#ifndef PATTERNS_H
#define PATTERNS_H

class Pattern {
    public:
        static void setLEDCount(int ledCount);    
        static int getLEDCount();  
        virtual void update();
        // virtual void transition();
        virtual void transitionIn();
        virtual void transitionOut();
        virtual void getPixel(int pixelIndex, HSVColor* result);  
    private:
        static int ledCount;
};

class SolidPattern : public Pattern {
    public:
        SolidPattern(HSVColor color);

        void getPixel(int pixelIndex, HSVColor* result) override;

        void setColor(HSVColor color);

        HSVColor getColor();

        void transitionIn() override;
        void transitionOut() override;

        void update() override;

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
        FirePattern(HSVColor litColor, HSVColor unlitColor);

        // void transition() override;
        void transitionIn() override;
        void transitionOut() override;

        void update() override;

        void getPixel(int pixelIndex, HSVColor* result) override;

    private:
        struct Flare {
            double energy;
            double position;
        };

        double extraEnergy;
        unsigned long lastTime;

        HSVColor unlitColor;
        HSVColor litColor;

        Flare flares[MAX_FLARES];
};

#define PROBE_DIFFUSION 1
#define PROBE_SPEED 0.02
class ProbePattern : public Pattern {
    public:
        ProbePattern(HSVColor color);
        // void transition() override;

        void getPixel(int pixelIndex, HSVColor* result) override;

        void transitionIn() override;
        void transitionOut() override;

        void update() override;

    private:
        HSVColor color;
};

#define RAINBOW_SPEED 0.02
class RainbowPattern : public Pattern {
    public:
        void getPixel(int pixelIndex, HSVColor* result) override;

        void transitionIn() override;
        void transitionOut() override;

        void update() override;
};
#endif