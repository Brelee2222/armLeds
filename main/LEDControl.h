#include "Patterns.h"

#define LED_PIN 2

#define LED_COUNT 75

class LEDMenu {
    public:
        LEDMenu();

        virtual void update();
        virtual void display();
        
    private:
        LEDMenu* parent;
        static LEDMenu* currentMenu; 

    protected:
        static void back();
};

namespace LEDControl {
    void begin();

    void update();

    void display();
}

namespace PatternSelection {
    // extern Pattern* patterns[4];
    extern Pattern* patterns[4];

    extern Pattern& currentPattern;

    void begin();

    void update();

    void transitionPattern(Pattern& newPattern);
}

namespace SwitchInterface {
    void begin();

    unsigned char getBitsValue(int bits);
}