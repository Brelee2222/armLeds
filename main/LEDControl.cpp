#include "LEDControl.h"
#include "Patterns.h"
#include "HSVTune.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int BIT_PINS[4] = {2, 3, 4, 5};

Pattern* patterns[PATTERNS_SIZE] = {
    new SolidPattern(HSVColor(0, 255, 255)),
    new FirePattern(HSVColor(7281, 254, 255), HSVColor(0, 255, 15)),
    new ProbePattern(HSVColor(0, 255, 255)),
    new RainbowPattern()
};

Pattern& currentPattern = *patterns[1];

unsigned char lastPatternIndex = 0;

LEDMenu::LEDMenu() {
    this->parent = LEDMenu::currentMenu;

    LEDMenu::currentMenu = this;
}
void LEDMenu::back() {
    LEDMenu* menu = LEDMenu::currentMenu;

    LEDMenu::currentMenu = menu->parent;

    delete menu;
}
LEDMenu* LEDMenu::currentMenu = new PatternSelectionMenu();

void PatternSelectionMenu::display() {}
void PatternSelectionMenu::update() {
    char currentPatternIndex = SwitchInterface::getBitsValue(3);

    if(lastPatternIndex != currentPatternIndex) {
        lastPatternIndex = currentPatternIndex;

        if(currentPatternIndex < PATTERNS_SIZE)
            transitionPattern(*patterns[currentPatternIndex]);
    }
}
void PatternSelectionMenu::transitionPattern(Pattern& newPattern) {
    currentPattern.transitionOut();
    (currentPattern = newPattern).transitionIn();
}

namespace LEDControl {
    void begin() {
        leds.begin();

        HSVTune::begin();
        SwitchInterface::begin();
        Pattern::setLEDCount(LED_COUNT);

        currentPattern.transitionIn();
    }

    void update() {
        HSVTune::update();
        LEDMenu::currentMenu->update();
    }

    void display() {
        HSVColor hsvModifier = HSVTune::getColorModifier();
        HSVColor pixelPatterColor;

        for(int ledIndex = LED_COUNT; ledIndex >= 0; ledIndex--) {
            currentPattern.getPixel(ledIndex, &pixelPatterColor);

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

        LEDMenu::currentMenu->display();

        if(leds.canShow())
            leds.show();
    }
}

namespace SwitchInterface {
    boolean previousBitValues[PATTERNS_SIZE] = {};

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

    bool updateBit(int bit) {
        return previousBitValues[bit] != (previousBitValues[bit] = !digitalRead(BIT_PINS[bit]));
    }
}