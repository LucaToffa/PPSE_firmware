#ifndef __LED__
#define __LED__

#include "Defines.h"
#include <Adafruit_NeoPixel.h>  

void setup_led_strip();
void reset_led_strip();
void example_led_strip();
void led_strip_set_color(int led, int r, int g, int b);
void led_strip_set_all(int r, int g, int b);
#endif // __LED__   