#include "Patterns.h"
#include "HSVTune.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define LED_PIN 2

#define LED_COUNT 75

#define BRIGHTNESS 230

#define frameDelay 10

Pattern* patterns[4] = {
    new SolidPattern(HSVColor(0, 255, 255)),
    new FirePattern(HSVColor(7281, 254, 255), HSVColor(0, 255, 15)),
    new ProbePattern(HSVColor(0, 255, 255)),
    new RainbowPattern()
};

Pattern& currentPattern = *patterns[0];

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    Pattern::setLEDCount(LED_COUNT);

    HSVTune::begin();

    leds.begin();

    currentPattern.transitionIn();
    
    Serial.begin(9600);
}
// SolidPattern currentPattern = SolidPattern(generateColor(125, 255, 255));

void loop()
{
    // void* ptr = malloc(1);

    // Serial.println((int) ptr);

    // free(ptr);

    HSVTune::update();

    currentPattern.update();

    HSVColor hsvModifier = HSVTune::getColorModifier();
    HSVColor pixelPatterColor;

    for(int ledIndex = leds.numPixels()-1; ledIndex >= 0; ledIndex--) {
        currentPattern.getPixel(ledIndex, &pixelPatterColor);

        HSVTransform::transformColor(&pixelPatterColor, hsvModifier);

        leds.setPixelColor(ledIndex, leds.ColorHSV(pixelPatterColor.hue, pixelPatterColor.saturation, pixelPatterColor.value));
    }

    if(leds.canShow())
        leds.show();

    delay(frameDelay);
}