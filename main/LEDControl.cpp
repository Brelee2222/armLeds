#include "LEDControl.h"
#include "Patterns.h"
#include "HSVTune.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


const HSVColor COLOR_MODIFIERS[3] = {
    HSVColor(0, 255, 230),
    HSVColor(43690, 255, 230)
};

const int BIT_PINS[4] = {7, 6, 5, 4};

Pattern* patterns[PATTERNS_SIZE] = {
    new SolidPattern(HSVColor(0, 255, 255)),
    new FirePattern(HSVColor(7281, 254, 255), HSVColor(0, 255, 15)),
    new ProbePattern(HSVColor(0, 255, 255)),
    new RainbowPattern(),
    new BlinkPattern(HSVColor(0, 255, 255))
};

unsigned char selectedPatternIndex = 0;
unsigned char selectedColorModIndex = 0;
unsigned char selectedBrightness = 3;

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
    if(SwitchInterface::updateBit(3)) {
        new ColorModifierMenu();
        return;
    }

    char currentPatternIndex = SwitchInterface::getBitsValue(3);
    if(currentPatternIndex < PATTERNS_SIZE && selectedPatternIndex != currentPatternIndex) {
        selectedPatternIndex = currentPatternIndex;

        transitionPattern(selectedPatternIndex);
    }
}
void PatternSelectionMenu::transitionPattern(char newPaternIndex) {
    patterns[selectedPatternIndex]->transitionOut();
    patterns[selectedPatternIndex = newPaternIndex]->transitionIn();
}

void ColorModifierMenu::display() {
    leds.setPixelColor(LED_COUNT - 1, 0xff00ff);
    leds.setPixelColor(LED_COUNT - 2, 0);

    int modifierPixelOffset = 0;
    for(HSVColor colorMod : COLOR_MODIFIERS) {
        modifierPixelOffset += 2;

        leds.setPixelColor(LED_COUNT - 1 - modifierPixelOffset, 0xff00ff);
        leds.setPixelColor(LED_COUNT - 2 - modifierPixelOffset, 0);
    }

    leds.setPixelColor(LED_COUNT - 2 - 2 * selectedColorModIndex, 0xaaaaaa);
}
void ColorModifierMenu::update() {
    // if(!SwitchInterface::updateBit(3)) {
    if(SwitchInterface::updateBit(3)) {
        this->back();
        return;
    }

    if(SwitchInterface::updateBit(2)) {
        new BrightnessMenu();
        return;
    }
    char currentColorModIndex = SwitchInterface::getBitsValue(2);

    if(currentColorModIndex < COLOR_MOD_SIZE && selectedColorModIndex != currentColorModIndex) {
        selectedColorModIndex = currentColorModIndex;
    }
}

void BrightnessMenu::display() {
    
}
void BrightnessMenu::update() {
    // if(!SwitchInterface::updateBit(2)) {
    if(SwitchInterface::updateBit(2)) {
        this->back();
        return; 
    }
    char currentBrightness = SwitchInterface::getBitsValue(2);

    if(selectedBrightness != currentBrightness) {
        selectedBrightness = currentBrightness;
    }
}

namespace LEDControl {
    void begin() {
        patterns[selectedPatternIndex]->transitionIn();
        leds.begin();

        HSVTune::begin();
        SwitchInterface::begin();
        Pattern::setLEDCount(LED_COUNT);
    }

    void update() {
        HSVTune::update();
        LEDMenu::currentMenu->update();
        patterns[selectedPatternIndex]->update();
    }

    void display() {
        HSVColor hsvModifier;
        if(selectedColorModIndex == 0)
            hsvModifier = HSVTune::getColorModifier();
        else
            hsvModifier = COLOR_MODIFIERS[selectedColorModIndex - 1];

        hsvModifier.value = (double) hsvModifier.value / 5 * (2 + selectedBrightness);

        HSVColor pixelPatterColor;

        Pattern& pattern = *patterns[selectedPatternIndex];

        for(int ledIndex = LED_COUNT; ledIndex >= 0; ledIndex--) {
            pattern.getPixel(ledIndex, &pixelPatterColor);

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
         
        for(int bitsPinIndex = bits; bitsPinIndex >= 0; bitsPinIndex--) {
            // Serial.println((int) !digitalRead(BIT_PINS[bitsPinIndex]));
            bitsValue = (bitsValue << 1) | !digitalRead(BIT_PINS[bitsPinIndex]);
        }

        return bitsValue;
    }

    bool updateBit(int bit) {
        return previousBitValues[bit] != (previousBitValues[bit] = !digitalRead(BIT_PINS[bit]));
        // return !digitalRead(BIT_PINS[bit]);
    }
}