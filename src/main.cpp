/**************************************************************************
 * Project:       PPSE board
 * Group: 
 * Date:          may 2023
 **************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>


#include "Defines.h"
#include "OLEDScreen.h"
#include "Buzzer.h"
#include "Buttons.h"
#include "LED.h"
#include "Accelerometer.h"

//variables
long unsigned int ledMillis = 0;
extern Adafruit_NeoPixel pixels;
extern Adafruit_SSD1306 display;
extern const unsigned char PROGMEM logo_bmp[];
// put function declarations here:
void poll_i2c();
void test_monitor();
void example_oled();
void loop_alive(long unsigned int period);

void setup() {
  pinMode(LED_ALIVE, OUTPUT);
  digitalWrite(LED_ALIVE, HIGH);
  setup_buzzer();
  Serial.begin(9600);
  setup_led_strip();
  delay(2000);  //delay to allow for serial monitor to connect
  Serial.println("Hello World");
  //setup_accelerometer(); //not working
  //init_display(); //could block code if display is not connected
  reset_led_strip();
  ledMillis = millis();
  
}

int do_buzzer = 0;
int do_alive = 0;
int do_strip = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if(do_alive){
    loop_alive(100);
  }
  if(do_strip){
    example_led_strip();
  }
  if(do_buzzer){
    loop_buzzer();
  }
  
  int pressed = read_buttons();
  switch(pressed){
    case BUTTON_UP:
      do_alive = 0;
      do_strip = 0;
      do_buzzer = 0;
      pixels.clear();
      pixels.show();
      break;
    case BUTTON_DOWN:
      do_buzzer = !do_buzzer;
      break;
    case BUTTON_LEFT:
      do_alive = !do_alive;
      break;
    case BUTTON_RIGHT: 
      do_strip = !do_strip;
      break;
    default:
      //do nothing
      break; 
  }

}

// put function definitions here:
void poll_i2c(){
  //search the entire address range for devices
  MbedI2C myWire(SDA, SCL);
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 12; address++ ){
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    myWire.beginTransmission(address);
    error = myWire.endTransmission();
    if(error == 0){
      Serial.print("I2C device found at address 0x");
      if(address < 16){
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    }else if(error == 4){
      Serial.print("Unknow error at address 0x");
      if(address < 16){
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
}

void test_monitor(){
  //open serial monitor and set baud rate to 9600
  Serial.begin(9600);
  //print to serial monitor in a loop
  while(true){
    Serial.println("Hello World!");
    delay(1000);
  }
}


void loop_alive(long unsigned int period){
  if(millis() - ledMillis > period) {
    ledMillis = millis();
    digitalWrite(LED_ALIVE, !digitalRead(LED_ALIVE));
  }
}



void example_oled(){
  //all the official examples here
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
