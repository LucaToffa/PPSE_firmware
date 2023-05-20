#include "LED.h"

Adafruit_NeoPixel pixels(STRIP_LENGHT, LED_STRIP, NEO_GRB + NEO_KHZ800); //strip file to control it

void setup_led_strip(){
  pixels.begin();
  pixels.setBrightness(50);
  pixels.show(); // Initialize all pixels to 'off'
}

void reset_led_strip(){
  pixels.clear();
  pixels.show();
}

void example_led_strip(){
  pixels.clear();
  for(int i=0; i < STRIP_LENGHT; i++) {
    pixels.setPixelColor(i, pixels.Color(i*5, 0, 15));
    pixels.show();
  }
}

void led_strip_set_color(int led, int r, int g, int b){
  pixels.setPixelColor(led, pixels.Color(r, g, b));
  pixels.show();
}