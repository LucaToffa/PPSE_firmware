/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>


#include "Defines.h"

void example_led_strip();
void example_adc();
void example_oled();
void example_eeprom();
void example_wifi();
void example_gps();
void example_acc();
void example_temperature();
void example_battery();
void example_buzzer();

//void setup_peripherals();
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RP, OUTPUT);
  digitalWrite(LED_RP, HIGH);
  delay(1000);
  digitalWrite(LED_RP, LOW);
  delay(1000);

  pixels.begin();
  //example_adc();
  Serial.begin(9600);

  init_display();
  
  testdrawline();      // Draw many lines

  testdrawrect();      // Draw rectangles (outlines)

  testfillrect();      // Draw rectangles (filled)

  testdrawcircle();    // Draw circles (outlines)

  testfillcircle();    // Draw circles (filled)

  testdrawroundrect(); // Draw rounded rectangles (outlines)

  testfillroundrect(); // Draw rounded rectangles (filled)

  testdrawtriangle();  // Draw triangles (outlines)

  testfilltriangle();  // Draw triangles (filled)

  testdrawchar();      // Draw characters of the default font

  testdrawstyles();    // Draw 'stylized' characters

  testscrolltext();    // Draw scrolling text

  testdrawbitmap();    // Draw a small bitmap image

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);

  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //switch case for the peripherals
  switch(Peripherals_state){
    case MAIN_MENU:
      //show main menu
      break;
    case WIFI_SENDING:
      //send data to the wifi
      break;
    case WIFI_RECEIVING:
      //receive data from the wifi
      break;
    case GPS_SENDING: 
      //send data to the gps
      break;
    case ACC_SENDING: 
      //send data to the accelerometer
      break;
    case LEDS_DATA: 
      //send data to the leds
      break;
    case TEMPERATURE_DATA: 
      //read data to the temperature sensor
      break;
    case BATTERY_DATA:
      //read data to the battery sensor
      break;
    case BUZZER_SIGNAL: 
      //send data to the buzzer
      break;
    case PERIPHERAL_ERROR:
      //send error message
      break;
    case PERIPHERAL_IDLE:
      //do nothing
      break; 
    default:
      //do nothing
      break;   
  }
}

void example_led_strip(){
  pixels.clear();
  for(int i=0; i < STRIP_LENGHT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(500);
  }
}

void example_adc(){
  printf("ADC Example, measuring GPIO26\n");

  adc_init();

  // Make sure GPIO is high-impedance, no pullups etc
  adc_gpio_init(26);
  // Select ADC input 0 (GPIO26) (0-3,ADC0-ADC3 = GPIO26-29 )
  adc_select_input(0);

  while (1) {
  // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
  const float conversion_factor = 3.3f / (1 << 12);
  uint16_t result = adc_read();
  printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);
  sleep_ms(500);
  }
}
