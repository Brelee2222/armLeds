#include "LEDControl.h"
#include "Patterns.h"
#include "HSVTune.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int BIT_PINS[4] = {2, 3, 4, 5};

LEDMenu::LEDMenu() {
    this->parent = LEDMenu::currentMenu;

    LEDMenu::currentMenu = this;
}

void LEDMenu::back() {
    LEDMenu* menu = LEDMenu::currentMenu;

    LEDMenu::currentMenu = menu->parent;

    delete menu;
}

namespace LEDControl {
    void begin() {
        leds.begin();

        HSVTune::begin();
        SwitchInterface::begin();
        PatternSelection::begin();
        Pattern::setLEDCount(LED_COUNT);
    }

    void update() {
        PatternSelection::update();
        HSVTune::update();
    }

    void display() {
        HSVColor hsvModifier = HSVTune::getColorModifier();
        HSVColor pixelPatterColor;

        for(int ledIndex = LED_COUNT; ledIndex >= 0; ledIndex--) {
            PatternSelection::currentPattern.getPixel(ledIndex, &pixelPatterColor);

            HSVTransform::transformColor(&pixelPatterColor, hsvModifier);

            leds.setPixelColor(
                ledIndex, 
                leds.ColorHSV(
                    pixelPatterColor.hue, 
                    pixelPatterColor.saturation, 
                    pixelPatterColor.value
                )
            );
        }

        if(leds.canShow())
            leds.show();
    }
}

namespace PatternSelection {
    Pattern* patterns[4] = {
        new SolidPattern(HSVColor(0, 255, 255)),
        new FirePattern(HSVColor(7281, 254, 255), HSVColor(0, 255, 15)),
        new ProbePattern(HSVColor(0, 255, 255)),
        new RainbowPattern()
    };

    Pattern& currentPattern = *patterns[1];

    unsigned char patternIndex = 0;

    void begin() {
        currentPattern.transitionIn();
    }

    void update() {
        char currentPatternIndex = SwitchInterface::getBitsValue(sizeof(patterns));

        if(patternIndex != currentPatternIndex) {
            patternIndex = currentPatternIndex;

            if(currentPatternIndex < sizeof(patterns))
                transitionPattern(*patterns[currentPatternIndex]);
        }
    }

    void transitionPattern(Pattern& newPattern) {
        currentPattern.transitionOut();
        (currentPattern = newPattern).transitionIn();
    }
}

namespace SwitchInterface {
    void begin() {      
        for(int bitPin : BIT_PINS)
            pinMode(bitPin, INPUT_PULLUP);
    }

    unsigned char getBitsValue(int bits) {
        unsigned char bitsValue = 0;
         
        for(int bitsPinIndex = 0; bitsPinIndex < bits; bitsPinIndex++)
            bitsValue = (bitsValue << 1) & !digitalRead(BIT_PINS[bitsPinIndex]);

        return bitsValue;
    }
}