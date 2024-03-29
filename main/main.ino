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
    new FirePattern(generateColor(4281, 254, 100), generateColor(0, 255, 100))
};

Pattern& currentPattern = *patterns[1];

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
    long currentTime = millis();
    long deltaTime = currentTime - lastTime;
    
    HSVTune::update();

    if(PatternSelection::update()) {
        currentPattern = *patterns[PatternSelection::selectedPatternIndex];

        currentPattern.transition();
    }

    currentPattern.update(deltaTime);

    HSVColor hsvModifier = HSVTune::getColorModifier();

    for(int ledIndex = leds.numPixels()-1; ledIndex >= 0; ledIndex--) {
        HSVColor pixelPatterColor = currentPattern.getPixel(ledIndex);

        // Serial.println(pixelPatterColor.hue);

        HSVColor pixelColor = HSVTransform::transformColor(pixelPatterColor, hsvModifier);

        // Serial.println(pixelColor.hue);

        leds.setPixelColor(ledIndex, leds.ColorHSV(pixelColor.hue, pixelColor.saturation, pixelColor.value));
    }

    leds.show();

    lastTime = currentTime;
    delay(frameDelay);
}