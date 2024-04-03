#include "LEDControl.h"

#define FRAME_DELAY 10

void setup() {
    // Serial.begin(9600);

    LEDControl::begin();
}

void loop() {
    // void* ptr = malloc(1);

    // Serial.println((int) ptr);

    // free(ptr);
    // delay(1000);
    LEDControl::update();

    LEDControl::display();

    delay(FRAME_DELAY);
}