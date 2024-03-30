#ifndef HSVTUNE_H
#define HSVTUNE_H

#define USE_HUE_OFFSET true

struct HSVColor {
    unsigned short hue;
    unsigned char saturation;
    unsigned char value;
};

HSVColor generateColor(short hue, char saturation, char value);

namespace HSVTransform {
    void transformColor(HSVColor* color, HSVColor transform);
}

namespace HSVTune {
    void begin(char huePin, char saturationPin, char valuePin);

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