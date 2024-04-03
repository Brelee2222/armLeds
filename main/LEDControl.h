#include "Patterns.h"

#define LED_PIN 2

#define LED_COUNT 80

#define PATTERNS_SIZE 5
#define COLOR_MOD_SIZE 3

/**
 * @brief LEDMenu is an abstract-like class meant to be extended to make the process of adding menus on an LED strip easy.
 * 
 * @note Hi :3
 * @note LEDMenu() and back() are the only two ways (at the moment) to switch between menus. Read their documentation to get an idea of how to use them.
*/
class LEDMenu {
    public:
        /**
         * @brief Initializes a new LED menu. 
         * 
         * @note When the LED menu is initialized, it assumes the currentMenu as its parent and becomes the new currentMenu.
        */
        LEDMenu();

        /**
         * @brief updates the menu
         * 
         * @note This function is mainly called on the current menu.
        */
        virtual void update();

        /**
         * @brief displays the menu
         * 
         * @note This function is mainly called on the current menu.
        */
        virtual void display();

        /**
         * @brief current menu
        */
        static LEDMenu* currentMenu; 
        
    private:
        /**
         * @brief The previous menu
        */
        LEDMenu* parent;

    protected:
        /**
         * @brief Makes currentMenu the current menu's parent.
         * 
         * @note The currentMenu is set to the currentMenu's previous, and the menu is deleted.
        */
        static void back();
};

class PatternSelectionMenu : public LEDMenu {
    public:
        void update() override;
        void display() override;

    private:
        /**
         * @brief calls transitionOut on the current pattern and transitionIn on the new pattern.
        */
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

/**
 * @brief This is what controls the LEDs.
 * 
 * @note the code might me a bit confusing and messy.
*/
namespace LEDControl {
    void begin();

    /**
     * @brief updates the led control
     * 
     * @note the code might me a bit confusing and messy.
    */
    void update();

    /**
     * @brief display the leds
     * 
     * @note the code might me a bit confusing and messy.
    */
    void display();
}

/**
 * @brief The switch interface creates a simple way to control things using a DIP switch.
 * 
 * @note The interface is currently mainly being used to detect changes in values.
*/
namespace SwitchInterface {
    void begin();

    /**
     * @brief stores the values read off of the pins assigned to the bits starting at 0 to the given bit into a byte/char. It reads the bits starting at 0 to the given bit and stores it into a byte.
    */
    unsigned char getBitsValue(int bits);

    /**
     * @brief updates the a given bit.
     * 
     * @returns a boolean for whether the bit has changed or not.
    */
    bool updateBit(int bit);

    /**
     * @brief returns the value currently being read from the pin assigned to the given bit.
    */
    bool getBit(int bit);
}