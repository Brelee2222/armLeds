#include "LEDControl.h"
#include "Patterns.h"
#include "HSVTune.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

/**
 * @brief List of color modifiers not including the HSVTune.
*/
const HSVColor COLOR_MODIFIERS[3] = {
    HSVColor(0, 255, 230),
    HSVColor(43690, 255, 230),
    HSVColor(21845, 255, 230)
};

/**
 * @brief list of patterns
 * 
 * @bug When you switch to FirePattern, then switch to ProbePattern, the code crashes. However, when you swap the indexes of RainbowPattern and ProbePattern, RainbowPattern will also crash the code.
*/
Pattern* patterns[PATTERNS_SIZE] = {
    new SolidPattern(HSVColor(0, 255, 255)),
    new FirePattern(HSVColor(7281, 254, 255), HSVColor(0, 255, 15)),
    new ProbePattern(HSVColor(0, 255, 255)),
    new RainbowPattern(),
    new BlinkPattern(HSVColor(0, 255, 255))
};

/**
 * @brief selected pattern 
*/
unsigned char selectedPatternIndex = 0;

/**
 * @brief selected color modifier 
*/
unsigned char selectedColorModIndex = 1;

/**
 * @brief selected brightness 
*/
unsigned char selectedBrightness = 3;

LEDMenu::LEDMenu() {
    this->parent = LEDMenu::currentMenu; // Sets the parent of the menu to current menu

    LEDMenu::currentMenu = this; // Sets the current menu to the menu
}
void LEDMenu::back() {
    LEDMenu* menu = LEDMenu::currentMenu;

    LEDMenu::currentMenu = menu->parent; // makes the current menu the parent

    delete menu; // deletes the menu
}
LEDMenu* LEDMenu::currentMenu = new PatternSelectionMenu();

void PatternSelectionMenu::display() {}
void PatternSelectionMenu::update() {
    if(SwitchInterface::updateBit(3)) { // checks if bit 3 has been changed
        new ColorModifierMenu();
        return;
    }

    char currentPatternIndex = SwitchInterface::getBitsValue(2); // Gets a value from the switch.
    if(currentPatternIndex < PATTERNS_SIZE && selectedPatternIndex != currentPatternIndex) {
        transitionPattern(currentPatternIndex);
    }
}
void PatternSelectionMenu::transitionPattern(char newPaternIndex) {
    patterns[selectedPatternIndex]->transitionOut();
    selectedPatternIndex = newPaternIndex;
    patterns[newPaternIndex]->transitionIn();
    // selectedPatternIndex = newPaternIndex;
}

void ColorModifierMenu::display() {
    HSVColor tuneColor = HSVTune::getColorModifier();
    leds.setPixelColor(LED_COUNT - 1, leds.ColorHSV(tuneColor.hue, tuneColor.saturation, tuneColor.value));
    leds.setPixelColor(LED_COUNT - 2, 0);

    int modifierPixelOffset = 0;
    for(HSVColor colorMod : COLOR_MODIFIERS) {
        modifierPixelOffset += 2;

        leds.setPixelColor(LED_COUNT - 1 - modifierPixelOffset, leds.ColorHSV(colorMod.hue, colorMod.saturation, colorMod.value));
        leds.setPixelColor(LED_COUNT - 2 - modifierPixelOffset, 0);
    }

    leds.setPixelColor(LED_COUNT - 2 - 2 * selectedColorModIndex, 0xaaaaaa);
}
void ColorModifierMenu::update() {
    // if(!SwitchInterface::updateBit(3)) {
    if(SwitchInterface::updateBit(3)) {
        char currentColorModIndex = SwitchInterface::getBitsValue(1);
        
        if(currentColorModIndex <= COLOR_MOD_SIZE && selectedColorModIndex != currentColorModIndex) {
            selectedColorModIndex = currentColorModIndex;
        }
        this->back();
        return;
    }

    if(SwitchInterface::updateBit(2)) {
        new BrightnessMenu();
        return;
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
    char currentBrightness = SwitchInterface::getBitsValue(1);

    if(selectedBrightness != currentBrightness) {
        selectedBrightness = currentBrightness;
    }
}

namespace LEDControl {
    void begin() {
        patterns[selectedPatternIndex]->transitionIn(); // initializes the starting pattern

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

        // Pattern& pattern = *patterns[selectedPatternIndex];

        for(int ledIndex = LED_COUNT; ledIndex >= 0; ledIndex--) {
            // pattern.getPixel(ledIndex, &pixelPatterColor);

            patterns[selectedPatternIndex]->getPixel(ledIndex, &pixelPatterColor);

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

/**
 * @brief list of pins assigned to bits.
*/
const int BIT_PINS[4] = {7, 6, 5, 4};

namespace SwitchInterface {
    bool previousBitValues[PATTERNS_SIZE] = {};

    void begin() {      
        int bitPinIndex = 0;
        for(int bitPin : BIT_PINS) {
            pinMode(bitPin, INPUT_PULLUP);
            previousBitValues[bitPinIndex++] = !digitalRead(bitPin);
        }
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
        return previousBitValues[bit] != (previousBitValues[bit] = getBit(bit));
        // return !digitalRead(BIT_PINS[bit]);
    }

    bool getBit(int bit) {
        return !digitalRead(BIT_PINS[bit]);
    }
}