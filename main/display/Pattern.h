PROGMEM const int LED_COUNT = 80;

class Pattern {
    public:
        /**
         * updates the pattern by a certain number of milliseconds
         */
        virtual void update(unsigned long millis);

        /**
         * Transitions the pattern in. Used to optimize memory.
         */
        virtual void initialize();

        /**
         * Transitions the pattern out. Used to optimize memory.
         */
        virtual void deinitialize();

        /**
         * Get a current pixel of the pattern at a certain index
         */
        virtual void getPixel(int pixelIndex, HSVColor* result);
};

/**
 * Transitions the pattern to a new one
 */
void changePattern(Pattern* pattern);

Pattern* currentPattern;