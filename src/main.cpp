/**************************************************************************
 * Project:       PPSE board
 * Group: 
 * Date:          may 2023
 **************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>


#include "Defines.h"
#include "Accelerometer.h"
 unsigned long ledMillis = 0;

 Adafruit_NeoPixel pixels(STRIP_LENGHT, LED_STRIP, NEO_GRB + NEO_KHZ800); //strip file to control it

 Debounce ButtonUp(BUTTON_UP, 50); //button class for combination readings, usable for every peripheral
 Debounce ButtonDown(BUTTON_DOWN, 50);
 Debounce ButtonRight(BUTTON_RIGHT, 50);
 Debounce ButtonLeft(BUTTON_LEFT, 50);

 ezBuzzer Buzzer(BUZZER);

//AT24C256 eeprom = AT24C256();

// Peripheral_state Peripherals_state = PERIPHERAL_IDLE;


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

void loop_alive(long unsigned int period);
int read_buttons();

//void setup_peripherals();
void setup() {
  pinMode(BUZZER, OUTPUT);
  Serial.begin(115200);
  setup_buzzer();
  pixels.begin();
  pinMode(LED_ALIVE, OUTPUT);
  digitalWrite(LED_ALIVE, HIGH);
  delay(1000);
  //setup_accelerometer(); //blocks everything
  digitalWrite(LED_ALIVE, LOW);
  delay(1000);
}

  

  /*
  // put your setup code here, to run once:
  pinMode(LED_RP, OUTPUT);
  digitalWrite(LED_RP, HIGH);
  delay(1000);
  digitalWrite(LED_RP, LOW);
  delay(1000);

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
  */
// }
 int do_alive = 0;
 int do_strip = 0;
 int do_buzzer = 0;
 int do_accelerometer = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if(do_alive){
    loop_alive(100);
  }
  if(do_strip){
    example_led_strip();
  }
  if(do_buzzer){
    STT_buzzer();
    //loop_buzzer();
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
  //loop_accelerometer(); 
  // /* digitalWrite(BUZZER, HIGH);
  // delay(2);
  // digitalWrite(BUZZER, LOW);
  // delay(2); */
  
  
 
  /*
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
  */
}

void example_led_strip(){
  pixels.clear();
  for(int i=0; i < STRIP_LENGHT; i++) {
    pixels.setPixelColor(i, pixels.Color(i*5, 0, 15));
    pixels.show();
    //delay(500);
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

void loop_alive(long unsigned int period){
  if(millis() - ledMillis > period) {
    ledMillis = millis();
    digitalWrite(LED_ALIVE, !digitalRead(LED_ALIVE));
  }
}


int read_buttons(){
  if(ButtonUp.read() == HIGH){
    //Serial.println("Button 1 pressed");
    return BUTTON_UP;
  }
  if(ButtonDown.read() == HIGH){
    //Serial.println("Button 2 pressed");
    return BUTTON_DOWN;
  }
  if(ButtonLeft.read() == HIGH){
    //Serial.println("Button 3 pressed");
    return BUTTON_LEFT;
  }
  if(ButtonRight.read() == HIGH){
    //Serial.println("Button 4 pressed");
    return BUTTON_RIGHT;
  }
  return 0;
}

void example_oled(){
  //put all examples here
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

//test code main

// // #define TEST_BUZZER 
// // #define TEST_TEMP
// // #define TEST_ACC
// // #define TEST_OLED
// // #define TEST_GPS
// // #define TEST_LED
// // #define TEST_EEPROM

// #ifdef TEST_BUZZER
// #include "Buzzer.h"
// #elif defined(TEST_TEMP)
// #include "Temp.h"
// #elif defined(TEST_ACC)
// #include "Accelerometer.h"
// #elif defined(TEST_OLED)
// #include "Oled.h"
// #elif defined(TEST_GPS)
// #include "GPS.h"
// #elif defined(TEST_LED)
// #include "Led.h"
// #elif defined(TEST_EEPROM)
// #include "Eeprom.h"
// #endif

// void setup() {
// #ifdef TEST_BUZZER
//   setup_buzzer();
// #elif defined(TEST_TEMP)
//   setup_temp();
// #elif defined(TEST_ACC)
//   setup_accelerometer();
// #elif defined(TEST_OLED)
//   setup_oled();
// #elif defined(TEST_GPS)
//   setup_gps();
// #elif defined(TEST_LED)
//   setup_led();
// #elif defined(TEST_EEPROM)
//   setup_eeprom();
// #endif
// }

// void loop() {
// #ifdef TEST_BUZZER
//   loop_buzzer();
// #elif defined(TEST_TEMP)
//   loop_temp();
// #elif defined(TEST_ACC)
//   loop_accelerometer();
// #elif defined(TEST_OLED)
//   loop_oled();
// #elif defined(TEST_GPS)
//   loop_gps();
// #elif defined(TEST_LED)
//   loop_led();
// #elif defined(TEST_EEPROM)
//   loop_eeprom();
// #endif
// }