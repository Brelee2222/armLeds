#include "HSVColor.h"

struct HSVColor;

#undef HSVColor()

HSVColor::HSVColor(
    unsigned short hue,
    unsigned char saturation,
    unsigned char value
) :
    hue(hue),
    saturation(saturation),
    value(value)
{
}

void HSVColor::transform(HSVColor* color, HSVColor* result) {
    result->hue = this->hue + color->hue;
};