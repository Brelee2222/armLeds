struct HSVColor {
    unsigned short hue;
    unsigned char saturation;
    unsigned char value;

    HSVColor(
        unsigned short hue,
        unsigned char saturation,
        unsigned char value
    );

    // transforms the color by another color and stores it in result
    void transform(HSVColor* color, HSVColor* result);
};