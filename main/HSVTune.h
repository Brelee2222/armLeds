#ifndef HSVTUNE_H
#define HSVTUNE_H

#define USE_HUE_OFFSET true

#define HUE_PIN A0
#define SAT_PIN A4
#define VAL_PIN A5

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

namespace HSVTransform {
    void transformColor(HSVColor *color, HSVColor transform);
}

namespace HSVTune {
    void begin();

    /**
     * @brief Set the half life of the analog pins to transition from its previous value to the current.
     * 
     * @param millis The half life in milliseconds.
     */
    void setHalfLife(double millis);

    double getHalfLife();

    void update();

    unsigned short getHueModifier();
    unsigned char getSaturationModifier();
    unsigned char getValueModifier();

    HSVColor getColorModifier();
}
#endif