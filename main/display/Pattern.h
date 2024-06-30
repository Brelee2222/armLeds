#include "Display.h"
#include "../HSV/HSVColor.h"

class Pattern {
    virtual void update(long time);

    virtual void initialize();
    virtual void deinitialize();

    virtual void getPixel(int pixelIndex, HSVColor* result);
};

void changePattern(Pattern& pattern);

Pattern currentPattern;