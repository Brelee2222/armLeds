#include "Patterns.h"

#ifndef LED_CONTROL 
#define LED_CONTROL
extern int BIT_PINS[5];

namespace LEDSelection {
    extern Pattern* patterns[4];

    extern Pattern& currentPattern; //starts off on cool fire pattern in case this fails

    extern int currentPatternIndex;

    void begin();

    void update();

    void transitionPattern(Pattern& newPattern);

    unsigned char getBitsValue(int bits);
}
#endif