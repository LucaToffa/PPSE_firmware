#include "Accelerometer.h"
#include "LED.h"
#include <Adafruit_NeoPixel.h>
Adafruit_MMA8451 mma = Adafruit_MMA8451();

uint16_t xyzRawAccel[3];
float xyzDataAccel[3];
sensors_event_t event;
accelerometer_data_t accel_data;    

void setup_accelerometer(){
    MbedI2C myWire(SDA, SCL);
    if(mma.begin(MMA8451Q_ADDR, &myWire)){
        Serial.println("MMA8451Q found!");
        //set leds to green
        for(int i=0; i < STRIP_LENGHT; i++) {
            led_strip_set_color(i, 0, 55, 0);
        }
    }else{
        Serial.println("Could not start MMA8451Q!");
        //set leds to red
        for(int i=0; i < STRIP_LENGHT; i++) {
            led_strip_set_color(i, 55, 0, 0);
        }
        delay(1000);
    }
    mma.setRange(MMA8451_RANGE_8_G);
}

void raw_read_accelerometer(){
    mma.read();
    Serial.print("X: "); Serial.print(mma.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(mma.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(mma.z); Serial.print("  ");
    Serial.println("m/s^2 ");
    xyzRawAccel[0] = mma.x; xyzRawAccel[1] = mma.y; xyzRawAccel[2] = mma.z;
    accel_data.rawx = mma.x; accel_data.rawy = mma.y; accel_data.rawz = mma.z;
}

void read_accelerometer(){
    mma.getEvent(&event);
    Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
    Serial.println("m/s^2 ");
    xyzDataAccel[0] = event.acceleration.x; xyzDataAccel[1] = event.acceleration.y; xyzDataAccel[2] = event.acceleration.z;
    accel_data.x = event.acceleration.x; accel_data.y = event.acceleration.y; accel_data.z = event.acceleration.z;

}

//return the value of the accelerometer in the specified axis
float read_accelerometer(int axis){
    mma.getEvent(&event);
    
    switch (axis)
    {
    case 0:
        return (event.acceleration.x);
        break;
    case 1:
        return (event.acceleration.y);
        break;
    case 2:
        return (event.acceleration.z);
        break;
    case 3:
        //return acc vector magnitude
        return (sqrt(pow(event.acceleration.x, 2) 
                + pow(event.acceleration.y, 2) 
                + pow(event.acceleration.z, 2)));
    default:
        return nanf("Not a valid axis");
        break;
    }

    return 0;
}

//change color of the leds depending on the accelerometer data
void setAccelLeds(){
    led_strip_set_color(0, 20, 20, 20);
    read_accelerometer();

    if(accel_data.x > 0){ //the value read is always 0.61 
        led_strip_set_color(1, 0, 50, 0);
        led_strip_set_color(2, 0, 50, 0);
    }
    if(accel_data.x == 0){
        led_strip_set_color(1, 50, 0, 0);
        led_strip_set_color(2, 50, 0, 0);
    }
    if(accel_data.x < 0){
        led_strip_set_color(1, 0, 0, 50);
        led_strip_set_color(2, 0, 0, 50);
    }
    if(accel_data.y > 0){
        led_strip_set_color(3, 0, 50, 0);
        led_strip_set_color(4, 0, 50, 0);
    }
    if(accel_data.y == 0){
        led_strip_set_color(3, 50, 0, 0);
        led_strip_set_color(4, 50, 0, 0);
    }
    if(accel_data.y < 0){
        led_strip_set_color(3, 0, 0, 50);
        led_strip_set_color(4, 0, 0, 50);
    }
    if(accel_data.z > 0){
        led_strip_set_color(5, 0, 50, 0);
        led_strip_set_color(6, 0, 50, 0);
    }
    if(accel_data.z == 0){
        led_strip_set_color(5, 50, 0, 0);
        led_strip_set_color(6, 50, 0, 0);   
    }
    if(accel_data.z < 0){
        led_strip_set_color(5, 0, 0, 50);
        led_strip_set_color(6, 0, 0, 50);
    }

    if(accel_data.x == accel_data.y  && accel_data.y  == accel_data.z){
        //set 0 and 7 to white
        led_strip_set_color(0, 50, 50, 50);
        led_strip_set_color(7, 50, 50, 50);
    }
}

