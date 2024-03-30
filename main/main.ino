#include "Patterns.h"
#include "HSVTune.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define LED_PIN 2

#define LED_COUNT 75

#define BRIGHTNESS 230

#define TRANSITION_HALFLIFE 250

#define huePin A0
#define saturationPin A4
#define valuePin A5

#define frameDelay 10

Pattern* patterns[3] = {
    new SolidPattern(generateColor(0, 255, 255)),
    new FirePattern(generateColor(7281, 254, 100), generateColor(0, 255, 5), LED_COUNT),
    new ProbePattern(generateColor(0, 255, 255), LED_COUNT),
};

Pattern& currentPattern = *patterns[2];

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    
    HSVTune::begin(huePin, saturationPin, valuePin);
    PatternSelection::begin(0, {0});

    HSVTune::setHalfLife(TRANSITION_HALFLIFE);

    leds.begin();

    currentPattern.transition();
    Serial.begin(9600);
}
// SolidPattern currentPattern = SolidPattern(generateColor(125, 255, 255));

long lastTime = 0;

void loop()
{
    // void* ptr = malloc(1);

    // Serial.println((int) ptr);

    // free(ptr);
    
    long currentTime = millis();
    long deltaTime = currentTime - lastTime;
    
    HSVTune::update();

    if(PatternSelection::update()) {
        currentPattern = *patterns[PatternSelection::selectedPatternIndex];

        currentPattern.transition();
    }

    currentPattern.update(deltaTime);

    HSVColor hsvModifier = HSVTune::getColorModifier();
    HSVColor pixelPatterColor;

    for(int ledIndex = leds.numPixels()-1; ledIndex >= 0; ledIndex--) {
        currentPattern.getPixel(ledIndex, &pixelPatterColor);

        HSVTransform::transformColor(&pixelPatterColor, hsvModifier);

        leds.setPixelColor(ledIndex, leds.ColorHSV(pixelPatterColor.hue, pixelPatterColor.saturation, pixelPatterColor.value));
    }

    if(leds.canShow())
        leds.show();

    lastTime = currentTime;
    delay(frameDelay);
}