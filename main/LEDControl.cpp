#include "LEDControl.h"

int BIT_PINS[5] = {0, 1, 2, 3};

namespace LEDSelection {

    Pattern* patterns[4] = {
        new SolidPattern(HSVColor(0, 255, 255)),
        new FirePattern(HSVColor(7281, 254, 255), HSVColor(0, 255, 15)),
        new ProbePattern(HSVColor(0, 255, 255)),
        new RainbowPattern()
    };

    Pattern& currentPattern = *patterns[1];

    unsigned char patternIndex = 1;
    
    void begin() {
        currentPattern.transitionIn();
        
        for(int bitPin : BIT_PINS)
            pinMode(bitPin, INPUT_PULLUP);
    }

    void update() {
        char currentPatternIndex = getBitsValue(sizeof(patterns));

        if(patternIndex != currentPatternIndex)
            transitionPattern(*patterns[patternIndex = currentPatternIndex]);
    }

    void transitionPattern(Pattern& newPattern) {
        currentPattern.transitionOut();
        (currentPattern = newPattern).transitionIn();
    }

    unsigned char getBitsValue(int bits) {
        unsigned char bitsValue = 0;
         
        for(int bitsPinIndex = 0; bitsPinIndex < bits; bitsPinIndex++)
            bitsValue = (bitsValue << 1) & !digitalRead(BIT_PINS[bitsPinIndex]);

        return bitsValue;
    }
}