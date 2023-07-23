/**************************************************************************
 * Project:       PPSE board
 * Group:         Luca Toffalori, Alessandro Cazzaniga, Filip Jaroszewski
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

enum enum_state main_state = IDLE;
//variables
TwoWire myWire(SDA, SCL);
Data_storage present_data;
int start_index = 0; 
int end_index = 0;
int len = 100;
int data_elements = 15;
Data_storage saved_data[100];
long unsigned int ledMillis = 0;
long unsigned int last_read_slow = 0;
long unsigned int last_read_fast = 0;
long unsigned int write_time = 0;
extern Adafruit_NeoPixel pixels;
extern Adafruit_SSD1306 display;
// put function declarations here:
void poll_i2c();
void test_monitor();
void loop_alive(long unsigned int period);
void loop_accelerometer(long unsigned int period);
void fn_update_page();
void move_data();
void data_reads();
void save_local(Data_storage present_data);
void send_serial();
void ring_alarm();

//list of y/n states for the selection
bool selection_datasave[3] = {true, false, false}; //local wifi usb 
bool selection_sensors[3] = {true, true, true}; //gps acc temp
bool selection_others[3] = {true, true, true}; //wifi buzz leds

void setup() {
  pinMode(LED_ALIVE, OUTPUT);
  digitalWrite(LED_ALIVE, LOW);
  Serial.begin(115200);
  init_buttons();
  main_state = IDLE;
  setup_buzzer();
  setup_led_strip();
  setup_gps();
  setup_temp();
  setup_accelerometer();
  init_display(); //could block code if display is not connected
  ledMillis = millis();
  last_read_slow = millis();
  last_read_fast = millis();
  write_time = millis();
  while(raw_read_buttons() != 0){ //avoid false button presses at startup
  }
  delay(1000); 
  //draw the main menu
  fn_IDLE();
}



void loop() {
  switch(main_state){//perform page related actions
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
  reset_buttons();//ready for new input
  move_data(); //r/w to/from peripherals
  ring_alarm(); //check for alarm conditions 
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
  //save data log
  if(selection_datasave[0] == true){ //local
    save_local(present_data);
  }
  //send data 
  if(selection_datasave[2] == true){ //usb
    send_serial();
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

void loop_accelerometer(long unsigned int period){
  if(millis() - ledMillis > period) {
    ledMillis = millis();
    //test_read_acc();
    read_accelerometer();
  }
}


//state machine functions to collect data and update pages
//just prototypes for now

void fn_update_page(){
  //update the page based on the current state

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
  //read data accoding to sensor states and timing
  if(millis() - last_read_fast > READING_TIME/FAST_SPEED){
      if(selection_sensors[1] == true){ 
        //read the accelerometer
        read_accelerometer();
        present_data.accx = read_accelerometer(0);
        present_data.accy = read_accelerometer(1);
        present_data.accz = read_accelerometer(2);
        setAccelLeds();
    }
    fn_update_page();
    last_read_fast = millis();
  }
  if(millis() - last_read_slow > READING_TIME) {
    if(selection_sensors[0] == true){
      //read the gps
      loop_gps();
      parseGpsData(); //does all the sprintf stuff, hopefully
      present_data.latitude = getLat().toFloat();
      present_data.longitude = getLon().toFloat();
      present_data.altitude = getAlt().toFloat();
      present_data.utc_time = getUtcTime();
      present_data.date = (getRawDate());
    } 

    if(selection_sensors[2] == true){
      //read the sensors
      read_temp();
      present_data.temp = get_temp();
    }
    last_read_slow = millis();
    fn_update_page();
  }

}

void ring_alarm(){
  //if eccessive value and if temp or acc are active
  if( (selection_sensors[1] == false) && (selection_sensors[2] == false) ){
    return; 
  }
  if(get_temp() > 50 || get_temp() < -20 || read_accelerometer(3) > 2.5){
    //set alarm off
    led_strip_set_all(50, 0, 0);
    alarm_tone();
    reset_led_strip();
  }

}

void save_local(Data_storage present_data){
  //save data to local memory in the structure at READING_TIME intervals
  if(millis() - write_time < WRITING_TIME){
    return;
  }
  saved_data[end_index] = present_data;
  end_index = (end_index + 1) % len;
  if(end_index == start_index){ //if the buffer is full move the start index
    start_index = (start_index + 1) % len;
  }
  write_time = millis();
  
}

void send_serial(){
  //send data to serial port when connected
  if(start_index == end_index){
    return; //no data to send
  }
  String log;
  Serial.println("Sending data");
  if(end_index > start_index){
    for (int i = start_index; i < end_index; i++){
      log = " Time: " + String(saved_data[i].utc_time) + " Date: " + String(saved_data[i].date) +
            " Lat: " + String(saved_data[i].latitude) + " Lon: " + String(saved_data[i].longitude) + 
            " Alt: " + String(saved_data[i].altitude) + " Temp: " + String(saved_data[i].temp) + 
            " Accx: " + String(saved_data[i].accx) + " Accy: " + String(saved_data[i].accy) +
            " Accz: " + String(saved_data[i].accz) + " Index: " + String(i);
      Serial.println(log);
    }
  }
  else if(end_index < start_index){
    for (int i = start_index; i < len; i++){
      log = " Time: " + String(saved_data[i].utc_time) + " Date: " + String(saved_data[i].date) +
            " Lat: " + String(saved_data[i].latitude) + " Lon: " + String(saved_data[i].longitude) + 
            " Alt: " + String(saved_data[i].altitude) + " Temp: " + String(saved_data[i].temp) + 
            " Accx: " + String(saved_data[i].accx) + " Accy: " + String(saved_data[i].accy) +
            " Accz: " + String(saved_data[i].accz) + " Index: " + String(i);
      Serial.println(log);
    }
    for (int i = 0; i < end_index; i++){
      log = " Time: " + String(saved_data[i].utc_time) + " Date: " + String(saved_data[i].date) +
            " Lat: " + String(saved_data[i].latitude) + " Lon: " + String(saved_data[i].longitude) + 
            " Alt: " + String(saved_data[i].altitude) + " Temp: " + String(saved_data[i].temp) + 
            " Accx: " + String(saved_data[i].accx) + " Accy: " + String(saved_data[i].accy) +
            " Accz: " + String(saved_data[i].accz) + " Index: " + String(i);
      Serial.println(log);
    }
  }
  else{
    for (int i = start_index; i < len; i++){
      log = " Time: " + String(saved_data[i].utc_time) + " Date: " + String(saved_data[i].date) +
            " Lat: " + String(saved_data[i].latitude) + " Lon: " + String(saved_data[i].longitude) + 
            " Alt: " + String(saved_data[i].altitude) + " Temp: " + String(saved_data[i].temp) + 
            " Accx: " + String(saved_data[i].accx) + " Accy: " + String(saved_data[i].accy) +
            " Accz: " + String(saved_data[i].accz) + " Index: " + String(i);
      Serial.println(log);
    }
  }
  Serial.println("Data sent");
  Serial.end();
  start_index = 0;
  end_index = 0;
}