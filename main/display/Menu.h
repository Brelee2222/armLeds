#include "Display.h"

class Menu {
    public:
        static Menu* currentMenu;

        Menu();

        virtual void update();
        virtual void display(Adafruit_NeoPixel* leds);
        static void back();

    private:
        Menu* parentMenu;
};