#include "Pattern.h"

Pattern* currentPattern;

void changePattern(Pattern* pattern) {
    currentPattern->deinitialize();

    currentPattern = pattern;

    pattern->initialize();
}