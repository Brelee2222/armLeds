#include "HSVColor.h"

struct HSVColor;

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

#define HSVColor() (0,0,0)

void HSVColor::transform(HSVColor* color, HSVColor* result) {
    result->hue = this->hue + color->hue;
};