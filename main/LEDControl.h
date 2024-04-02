#include "Patterns.h"

#define LED_PIN 2

#define LED_COUNT 75

#define PATTERNS_SIZE 5

class LEDMenu {
    public:
        LEDMenu();

        virtual void update();
        virtual void display();
        static LEDMenu* currentMenu; 
        
    private:
        LEDMenu* parent;

    protected:
        static void back();
};

class PatternSelectionMenu : public LEDMenu {
    public:
        void update() override;
        void display() override;

    private:
        void transitionPattern(char newPaternIndex);
};

class ColorModifierMenu : public LEDMenu {
    public:
        void update() override;
        void display() override;
};

class BrightnessMenu : public LEDMenu {
    public:
        void update() override;
        void display() override;
};

namespace LEDControl {
    void begin();

    void update();

    void display();
}

namespace SwitchInterface {
    void begin();

    unsigned char getBitsValue(int bits);

    bool updateBit(int bit);
}