#pragma once

#include "Pattern.h"
#include "../HSV/HSVColor.h"

#define data_pin 12

PROGMEM const int LED_COUNT = 80;

void begin();

void update();

void display();