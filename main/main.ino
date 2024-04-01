#include "Patterns.h"
#include "HSVTune.h"
#include "LEDControl.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define LED_PIN 2

#define LED_COUNT 75

#define BRIGHTNESS 230

#define FRAME_DELAY 10

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    LEDSelection::begin();
    Pattern::setLEDCount(LED_COUNT);

    HSVTune::begin();

    leds.begin();
    
    Serial.begin(9600);
}
// SolidPattern currentPattern = SolidPattern(generateColor(125, 255, 255));

void loop()
{
    // void* ptr = malloc(1);

    // Serial.println((int) ptr);

    // free(ptr);

    LEDSelection::update();
    HSVTune::update();

    Pattern& currentPattern = LEDSelection::currentPattern;

    HSVColor hsvModifier = HSVTune::getColorModifier();
    HSVColor pixelPatterColor;

    for(int ledIndex = leds.numPixels()-1; ledIndex >= 0; ledIndex--) {
        currentPattern.getPixel(ledIndex, &pixelPatterColor);

        HSVTransform::transformColor(&pixelPatterColor, hsvModifier);

        leds.setPixelColor(ledIndex, leds.ColorHSV(pixelPatterColor.hue, pixelPatterColor.saturation, pixelPatterColor.value));
    }

    if(leds.canShow())
        leds.show();

    delay(FRAME_DELAY);
}