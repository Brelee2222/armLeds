#include <Adafruit_NeoPixel.h>
#include "HSVTune.h"

#ifndef PATTERNS_H
#define PATTERNS_H

class Pattern {
    public:
        /*!
            @brief  Sets the number of LEDs being used.

            @note   This is mainly used as an "easy access" 
                    for patterns that depend on LEDCount to 
                    work such as FirePattern which uses it 
                    to mirror the flame, and ProbePattern 
                    which uses it to loop the probe glow.

            @param  ledCount The number of LEDs being used.

            @see    getLEDCount
        */
        static void setLEDCount(int ledCount);

        /*!
            @brief  Returns the LEDCount set by setLEDCount.
                    
            @note   This is mainly being used as "easy access" 
                    for patterns that depend on LEDCount to 
                    work such as FirePattern which uses it to 
                    mirror the flame, and ProbePattern which 
                    uses it to loop the probe glow.
            
            @return int -- LEDCount
         */
        static int getLEDCount();  

        /*!
            @brief  Updates the pattern

            @note   Can be used for patterns that have 
                    complex/recursive animations such as 
                    FirePattern.
         */
        virtual void update();

        /*!
            @brief  A function to initialize/transition patterns. 

            @note   This can be used to keep patterns that depend 
                    on time from breaking after they have not been 
                    updated after some period of time (if that makes sense).
                    This can also be used to prevent memory leaks 
                    by initializing memory for the pattern only when 
                    it is being used. 

            @see    transitionOut
         */
        virtual void transitionIn();

        /*!
            @brief   A function to deinitialize patterns
        
            @note    This function helps to prevent memory leaks by 
                    allocating and deinitializing patterns when they 
                    are no being used.

            @see     transitionIn
         */
        virtual void transitionOut();

        /*!
            @brief Writes the hue, saturation, and value of the pixel at index {pixelIndex} in the pattern to the pointer of result.

            @param pixelIndex The pixel index/position
            @param result The HSVColor to write to.

            @see HSVColor
         */
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

    private:
        HSVColor color;
};

/**
 * @brief Fire pattern for Flaming Chickens WOOOOOO!
 * @note This pattern works by simulating flares that make up a fire by having the flares "light up" LEDs.
 * There is a maximum number of flares can be exist at once, 
 * which is mainly used to cut down on the amount of processing that needs to be done. 
 * 
 * The max energy limits how much flares can propogate, how fast they rise, and how bright they are.
 * 
 * Flare lifespan was orignally used to control how fast energy diminishes within the flares, 
 * but was changed to be control the speed of the flares.
 * 
 * Flare speed is speed of flares._.
 * 
 * Light diffusion controls how much light the flares output.
 */
class FirePattern : public Pattern {
    #define MAX_FLARES 5
    #define MAX_ENERGY 2.5
    #define FLARE_LIFESPAN 5
    #define FLARE_SPEED 0.1
    #define LIGHT_DIFFUSION 0.5

    public:
        /**
         * @brief Construct a new Fire Pattern
         * 
         * @param litColor The color of an LED when the flares completely/fully light it up.
         * @param unlitColor The color of an LED when no flares are lighting it up.
         */
        FirePattern(HSVColor litColor, HSVColor unlitColor);

        void transitionIn() override;

        void update() override;

        void getPixel(int pixelIndex, HSVColor* result) override;

    private:

        /**
         * @brief The struct of a flare. It contains the values that control it.
         */
        struct Flare {
            double energy;
            double position;
        };

        /**
         * @brief The amount of usable energy.
         */
        double extraEnergy;

        /**
         * @brief Used to calculate delta time (change in time) to see how the flares should move.
         */
        unsigned long lastTime;

        HSVColor unlitColor;
        HSVColor litColor;

        /**
         * @brief array of flares
         */
        Flare flares[MAX_FLARES];
};

/**
 * @brief A pattern where a light probes across the LED strip.
 * 
 * @note Probe speed is how fast the light moves across the strip.
 * 
 * Probe diffusion is how much the light spreads.
 */
class ProbePattern : public Pattern {
    #define PROBE_DIFFUSION 1
    #define PROBE_SPEED 0.02

    public:
        /**
         * @brief Construct a new Probe Pattern
         * 
         * @param color the color of the light
         */
        ProbePattern(HSVColor color);

        void getPixel(int pixelIndex, HSVColor* result) override;

    private:
        HSVColor color;
};

/**
 * @brief The flashiest yet second simplest pattern in this project code!
 * 
 * @note The rainbow speed controls how fast the rainbow changes.
 */
class RainbowPattern : public Pattern {
    #define RAINBOW_SPEED 0.02

    public:
        void getPixel(int pixelIndex, HSVColor* result) override;
};

/**
 * @brief This pattern turns the LEDs on and off at once (I might add pulsing to make it less distracting)
 * 
 * @note The frequency controls how fast the LEDs turn on and off.
 */
class BlinkPattern : public Pattern {
    #define BLINK_FREQUENCY 500

    public:
        BlinkPattern(HSVColor color);

        void getPixel(int pixelIndex, HSVColor* result) override;
    private:
        HSVColor color;
};
#endif