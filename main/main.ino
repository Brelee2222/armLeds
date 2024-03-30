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

Pattern* patterns[2] = {
    new SolidPattern(generateColor(0, 255, 255)),
    new FirePattern(generateColor(7281, 254, 100), generateColor(0, 255, 5), LED_COUNT)
};

Pattern& currentPattern = *patterns[1];

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    
    HSVTune::begin(huePin, saturationPin, valuePin);
    PatternSelection::begin(0, {0});

    HSVTune::setHalfLife(TRANSITION_HALFLIFE);

    leds.begin();

    currentPattern.transitionIn();
    Serial.begin(9600);
}
// SolidPattern currentPattern = SolidPattern(generateColor(125, 255, 255));

unsigned long lastTime = 0;

void loop()
{
    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime - lastTime;
    
    HSVTune::update();

    if(PatternSelection::update()) {
        currentPattern.transitionOut();

        currentPattern = *patterns[PatternSelection::selectedPatternIndex];
        
        currentPattern.transitionIn();
    }

    currentPattern.update(deltaTime);

    HSVColor hsvModifier = HSVTune::getColorModifier();
    HSVColor pixelPatterColor;

    for(int ledIndex = LED_COUNT-1; ledIndex >= 0; ledIndex--) {
        currentPattern.getPixel(ledIndex, &pixelPatterColor);

        HSVTransform::transformColor(&pixelPatterColor, hsvModifier);

        leds.setPixelColor(ledIndex, leds.ColorHSV(pixelPatterColor.hue, pixelPatterColor.saturation, pixelPatterColor.value));
    }

    delete &pixelPatterColor;
    delete &hsvModifier;

    leds.show();

    lastTime = currentTime;

    delay(max((long) (frameDelay + currentTime - millis()), 0));
}