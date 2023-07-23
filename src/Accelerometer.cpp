#include "Accelerometer.h"
#include "LED.h"
#include "Defines.h"
#include <Arduino.h>
#include <Wire.h>

extern TwoWire myWire; 
//sensors_event_t event;
accelerometer_data_t accel_data;
mma8451_range_t range = MMA8451_RANGE_8_G;

extern bool selection_datasave[3]; //local wifi usb 
extern bool selection_sensors[3]; //gps acc temp
extern bool selection_others[3]; //wifi buzz leds

void setRange(int range) {
  myWire.beginTransmission(MMA8451_ADDRESS);
  myWire.write(MMA8451_XYZ_DATA_CFG_REG);
  myWire.write(range); // Set the full-scale range to +/- 2-4-8g
  myWire.endTransmission(false);
}

int getRange() {
  myWire.beginTransmission(MMA8451_ADDRESS);
  myWire.write(MMA8451_XYZ_DATA_CFG_REG);
  myWire.endTransmission(false);
  myWire.requestFrom(MMA8451_ADDRESS, 1);
  int rangeSetting = myWire.read();
  return rangeSetting;
}

void setActivityState(int state) { //1 for active, 0 for standby
  myWire.beginTransmission(MMA8451_ADDRESS);
  myWire.write(MMA8451_CTRL_REG1);
  myWire.write(state); // Set the accelerometer to active mode
  if(selection_others[2] == true){
    led_strip_set_all(0, 155, 0);
  }
  myWire.endTransmission();
}

void setup_accelerometer(){
    myWire.begin();
    setActivityState(0);
    // Configure the MMA8451 accelerometer
    range = MMA8451_RANGE_8_G;
    setRange(range);
    setActivityState(1);
	if(selection_others[2] == true){
		led_strip_set_all(0,155,0);
	}
    delay(100);
    reset_led_strip();
}

void stop_accelerometer(){
    setActivityState(0);
    //set leds to red
    led_strip_set_all(33, 0, 0);
}

void raw_read_accelerometer(){
    // Read the X, Y, and Z acceleration data
    myWire.beginTransmission(MMA8451_ADDRESS);
    myWire.write(MMA8451_OUT_X_MSB);
    myWire.endTransmission(false);
    myWire.requestFrom(MMA8451_ADDRESS, 6);
    
    accel_data.rawx = (myWire.read() << 8) | myWire.read();
    accel_data.rawy = (myWire.read() << 8) | myWire.read();
    accel_data.rawz = (myWire.read() << 8) | myWire.read();
}

void read_accelerometer(){
    if(selection_sensors[1] == false){
        return;
    }
    raw_read_accelerometer();
    // Convert the raw data to acceleration values (14 bit res)
    accel_data.rawx >>= 2;
    accel_data.rawy >>= 2;
    accel_data.rawz >>= 2;

    uint16_t divider = 1;
    if (range == MMA8451_RANGE_8_G){
        divider = 1024;
    }
    if (range == MMA8451_RANGE_4_G){
        divider = 2048;
    }
    if (range == MMA8451_RANGE_2_G){
        divider = 4096;
    }
    accel_data.x = (float)accel_data.rawx / divider;
    accel_data.y = (float)accel_data.rawy / divider;
    accel_data.z = (float)accel_data.rawz / divider;
}

//return the value of the accelerometer in the specified axis or the module
float read_accelerometer(int axis){
    switch (axis)
    {
    case 0:
        return accel_data.x;
    case 1:
        return accel_data.y;
    case 2:
        return accel_data.z;
    case 3:
        //return acc vector magnitude
        return (sqrt(pow(accel_data.x, 2) 
                + pow(accel_data.y, 2) 
                + pow(accel_data.z, 2)));
    default:
        return nanf("Not a valid axis");
    }
    return 0;
}

//change color of the leds depending on the accelerometer data
//    7 0 
// 6       1
// 5       2
//    4 3
void setAccelLeds(){
    if(selection_others[2] == false){
        return; //exit if leds are not active
    }
    //green > 0, blue < 0, red = 0 

    if(accel_data.x >= 0.01){
        led_strip_set_color(1, 0, 50, 0);
        led_strip_set_color(2, 0, 50, 0);
    }
    else if(accel_data.x <= -0.01){
        led_strip_set_color(1, 0, 0, 50);
        led_strip_set_color(2, 0, 0, 50);
    }
	else{
        led_strip_set_color(1, 50, 0, 0);
        led_strip_set_color(2, 50, 0, 0);
    }

    if(accel_data.y >= 0.01){
        led_strip_set_color(3, 0, 50, 0);
        led_strip_set_color(4, 0, 50, 0);
    }
    else if(accel_data.y <= -0.01){
        led_strip_set_color(3, 0, 0, 50);
        led_strip_set_color(4, 0, 0, 50);
    }
	else{
        led_strip_set_color(3, 50, 0, 0);
        led_strip_set_color(4, 50, 0, 0);
    }

    if(accel_data.z >= 0.01){
        led_strip_set_color(5, 0, 50, 0);
        led_strip_set_color(6, 0, 50, 0);
    }
    else if(accel_data.z <= -0.01){
        led_strip_set_color(5, 0, 0, 50);
        led_strip_set_color(6, 0, 0, 50);
    }
	else{
        led_strip_set_color(5, 50, 0, 0);
        led_strip_set_color(6, 50, 0, 0);   
    }

    //white = free fall
    if(accel_data.x == accel_data.y  && accel_data.y  == accel_data.z){
        led_strip_set_color(0, 50, 50, 50);
        led_strip_set_color(7, 50, 50, 50);
    }
}
