#ifndef HSVTUNE_H
#define HSVTUNE_H

/**
 * @brief This struct is used to define color through Hue, Saturation, and Value
 * 
 * @note It is used everywhere in the code and is the main way color is defined.
 * Hue is a value between 0-65535,
 * Saturation is a value between 0-255,
 * Value is a value between 0-255
 */
struct HSVColor {
    HSVColor(
        unsigned short hue,
        unsigned char saturation,
        unsigned char value
    );

    HSVColor();

    unsigned short hue;
    unsigned char saturation;
    unsigned char value;
};

/**
 * @brief As is suggests, this namespace is used to transform colors.
 */
namespace HSVTransform {
    /**
     * @brief Transforms a given color by another color.
     * 
     * @param color the color that will be transformed.
     * @param transform the color to transform by.
     */
    void transformColor(HSVColor *color, HSVColor transform);
}

/**
 * @brief This namespace contains stuff to tune the color of the LEDs.
 * 
 * @note 
 */
namespace HSVTune {
    /**
     * @brief When the USE_HUE_OFFSET is set to true, HSVTune's hue will always start off at 0 when the arduino is reset.
     */
    #define USE_HUE_OFFSET true

    /**
     * @brief The TRANSITION_HALFLIFE makes it so that the hue, saturation, and value of the HSVTune change smoothly so that it stays relatively stable and does not flicker.
     * 
     * @note The half life is how long it will take (in milliseconds) for the HSVTune values to become the average of it and the actual HSV values read off of analog.
     */
    #define TRANSITION_HALFLIFE 400

    #define HUE_PIN A0
    #define SAT_PIN A4
    #define VAL_PIN A5

    /**
     * @brief initializes HSVTune
     */
    void begin();

    /**
     * @brief updates/ticks HSVTune
     */
    void update();


    unsigned short getHueModifier();
    unsigned char getSaturationModifier();
    unsigned char getValueModifier();

    /**
     * @brief Gets the Hue, Saturation, and Value modifers and puts it in a HSVColor struct.
     * 
     * @return HSVColor the HSVColor struct containing the modifiers.
     */
    HSVColor getColorModifier();
}
#endif