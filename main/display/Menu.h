#include "Display.h"

class Menu {
    public:
        virtual void update();
        virtual void display(Adafruit_NeoPixel* leds);
        void back();
        void openMenu(Menu* menu);

    private:
        Menu* previousMenu;
};

Menu* currentMenu;