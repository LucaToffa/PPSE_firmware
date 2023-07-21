/**************************************************************************
 * Project:       PPSE board
 * Group:         Luca Toffalori, Alessandro Cazzaniga, Filip ???
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
#include "AnalogController.h"
#include "OLEDScreen.h"
#include "WiFiModule.h"
#include "GPS.h"
#include "FSM_screen.h"

extern enum enum_state current_state;
enum enum_state main_state = IDLE;
//variables
TwoWire myWire(SDA, SCL);
All_data present_data;
String input = "";
char input_char;
long unsigned int ledMillis = 0;
long unsigned int last_read_slow = 0;
long unsigned int last_read_fast = 0;
extern Adafruit_NeoPixel pixels;
extern Adafruit_SSD1306 display;
// put function declarations here:
void poll_i2c();
void test_monitor();
//void example_oled();
void loop_alive(long unsigned int period);
void loop_accelerometer(long unsigned int period);
void fn_update_page();
void move_data();
void data_reads();
void ring_alarm();

//list of y/n states for the selection
bool selection_datasave[3] = {true, false, false}; //local wifi usb, function parameter 
bool selection_sensors[3] = {true, true, true}; //gps acc temp, if in the call function 
bool selection_others[3] = {true, true, true}; //wifi buzz leds, function parameter

void setup() {
  
  init_buttons();
  current_state = IDLE;
  main_state = IDLE;
  Serial.begin(115200);
  delay(2000);  //delay to allow for serial monitor to connect
  Serial.println("Hello World");
  setup_buzzer();
  setup_led_strip();
  setup_gps();
  setup_temp();
  setup_accelerometer();
  init_display(); //could block code if display is not connected
  ledMillis = millis();
  last_read_slow = millis();
  last_read_fast = millis();
  while(raw_read_buttons() != 0){

  }
  
  delay(1000); 
  //draw the main menu
  fn_IDLE();
  Serial.println("Setup complete");
}



void loop() {
  // while(1){
  //   char temp_string[256];
  //   read_accelerometer();
  //   Serial.println("1");
  //   sprintf(temp_string, " X: %f Y:%f Z:%f", read_accelerometer(0), read_accelerometer(1), read_accelerometer(2));  
  //   present_data.accx = String(temp_string);
  //   Serial.println("2");
  //   sprintf(temp_string, " module: %f", read_accelerometer(3));
  //   Serial.println("3");
  //   present_data.accy = String(temp_string);
  //   Serial.println("4");
  //   Serial.println(present_data.accx);
  //   Serial.println(present_data.accy);
  //   delay(1000);
  // }
  // call the function that handles the current state
  //swtch case for the state machine that handles current_state
  Serial.println("loop state: " + String(main_state) + "\n");
  //main_state = current_state;
  switch(main_state){
    case IDLE:
      main_state = fn_IDLE();
      break;
    case HOME:
      main_state = fn_HOME();
      break;
    case DATA_RT:
      main_state = fn_DATA_RT();
      break;
    case GPS:
      main_state = fn_GPS();
      break;
    case ACC:
      main_state = fn_ACC();
      break;
    case DATA_SAVE:
      main_state = fn_DATA_SAVE();
      break;
    case SETTINGS:
      main_state = fn_SETTINGS();
      break;
    case SENSORS:
      main_state = fn_SENSORS();
      break;
    case OTHERS:
      main_state = fn_OTHERS();
      break;
    default:
      Serial.println("Error: unknown main state");
      break;
  }
  reset_buttons();
  data_reads();
  ring_alarm();
  //move_data(); //the function that handles all data r/w
}



// put function definitions here:
void poll_i2c(){
  //search the entire address range for devices
  MbedI2C myWire(SDA, SCL);
  myWire.begin();
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 128; address++ ){
    Serial.println("Scanning address: 0x" + String(address, HEX) + "...");
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    myWire.beginTransmission(address);
    Serial.println("begin transmission");
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
    else{
      Serial.print("No device found at address 0x");
    }
  }
}

void move_data(){
  data_reads(); //read data from peripherals
  //fn_update_page(); //update the current page based on the current state, only if new data is available
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

void loop_accelerometer(long unsigned int period){
  if(millis() - ledMillis > period) {
    ledMillis = millis();
    //test_read_acc();
    read_accelerometer();
  }
}

// void example_oled(){
//   //all the official examples here
//   init_display();  
//   testdrawline();      // Draw many lines
//   testdrawrect();      // Draw rectangles (outlines)
//   testfillrect();      // Draw rectangles (filled)
//   testdrawcircle();    // Draw circles (outlines)
//   testfillcircle();    // Draw circles (filled)
//   testdrawroundrect(); // Draw rounded rectangles (outlines)
//   testfillroundrect(); // Draw rounded rectangles (filled)
//   testdrawtriangle();  // Draw triangles (outlines)
//   testfilltriangle();  // Draw triangles (filled)
//   testdrawchar();      // Draw characters of the default font
//   testdrawstyles();    // Draw 'stylized' characters
//   testscrolltext();    // Draw scrolling text
//   testdrawbitmap();    // Draw a small bitmap image
//   // Invert and restore display, pausing in-between
//   display.invertDisplay(true);
//   delay(1000);
//   display.invertDisplay(false);
//   delay(1000);
//   testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
// }

//state machine functions to collect data and update pages
//just prototypes for now

void fn_update_page(){
  //update the page based on the current state
  /*
  str1.replace(position,length,str2); --no
  str1.replace(const String& find, const String& replace); 
  maybe way to update lines
  */
  switch(main_state){
    //assign the correct present_data strings to the correct lines
    case IDLE: //no data to update
    case HOME:
    case DATA_SAVE:
    case SETTINGS:
    case SENSORS:
    case OTHERS:
      break;
    
    case DATA_RT:
    case GPS:
    case ACC:
      //update current data page
      update_page(main_state);
      break;
      
    default:
      Serial.println("Error: unknown update state");
      break;
  }
}

//make so that data is read from present_data instead of directly
//read data from peripherals and update the data structure
void data_reads(){
  char temp_string[256];
  //read data accoding to sensor states and timing
  if(millis() - last_read_fast > READING_TIME/FAST_SPEED){
      if(selection_sensors[1] == true){ 
        //read the accelerometer
        read_accelerometer();
        // String present_data.accx = String(accel_data.x, 3);
        // String present_data.accy = String(accel_data.y, 3);
        // String present_data.accz = String(accel_data.z, 3);
        sprintf(temp_string, " X: %f Y:%f Z:%f", read_accelerometer(0), read_accelerometer(1), read_accelerometer(2));  
        present_data.accx = String(temp_string);
        sprintf(temp_string, " module: %f", read_accelerometer(3));
        present_data.accy = String(temp_string);
        setAccelLeds();
    }
    fn_update_page();
    last_read_fast = millis();
  }
  if(millis() - last_read_slow > READING_TIME) {
    //individulal data peges need update too
    if(selection_sensors[0] == true){
      //read the gps
      loop_gps();
      parseGpsData(); //does all the sprintf stuff, hopefully
    } 

    if(selection_sensors[2] == true){
      //read the sensors
      float temp = read_temp();
      //String present_data.temp = String(temp, 1);
      //sprintf(temp_string, " Temp: %f", temp);
      present_data.temp = String(temp);
    }
    last_read_slow = millis();
    fn_update_page();
  }

}

void ring_alarm(){
  //if eccessive value and if temp or acc are active
  if( (selection_sensors[1] == false) || (selection_sensors[2] == false) ){
    return;
  }
  if(get_temp() > 50 || get_temp() < -20 || read_accelerometer(3) > 3){
    //set alarm off
    led_strip_set_all(50, 0, 0);
    alarm_tone();
    reset_led_strip();
  }

}
/*

int LED = 13;
int x = 0;
void setup() {
  // Define the LED pin as Output
  pinMode (LED, OUTPUT);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}
void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C
}
void loop() {
  //If value received is 0 blink LED for 200 ms
  if (x == '0') {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
  }
  //If value received is 3 blink LED for 400 ms
  if (x == '3') {
    digitalWrite(LED, HIGH);
    delay(400);
    digitalWrite(LED, LOW);
    delay(400);
  }
}

*/