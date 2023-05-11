#ifndef __ACCELEROMETER__
#define __ACCELEROMETER__

#include <Wire.h>
#include "hardware/i2c.h"
#include "Defines.h"
//i2c_inst_t* i2c = i2c0;

// Define the I2C address of the MMA8451Q accelerometer
#define MMA8451Q_ADDR   0x1D

// Define the addresses of the MMA8451Q registers
#define MMA8451Q_CTRL_REG1      0x2A
#define MMA8451Q_OUT_X_MSB      0x01
#define MMA8451Q_OUT_Y_MSB      0x03
#define MMA8451Q_OUT_Z_MSB      0x05

// Initialize the I2C bus
extern MbedI2C myWire;

int16_t read16(uint8_t addr);
//void setup_accelerometer();
//void loop_accelerometer(); 


// Define a function to read a 16-bit value from two consecutive registers
int16_t read16(uint8_t addr) {
    
    myWire.beginTransmission(MMA8451Q_ADDR);
    myWire.write(addr);
    myWire.endTransmission(false);
    myWire.requestFrom(MMA8451Q_ADDR, 2);
    while (myWire.available() < 2);
    uint8_t lsb = myWire.read();
    uint8_t msb = myWire.read();
    return (int16_t)((msb << 8) | lsb);
}

void setup_accelerometer() {
    // Start the serial communication
    //Serial.begin(9600);

    // Start the I2C bus
    //I2C.begin(SDA, SCL);

    // Set the MMA8451Q to active mode
    myWire.beginTransmission(MMA8451Q_ADDR);
    myWire.write(MMA8451Q_CTRL_REG1);
    myWire.write(0x01);
    myWire.endTransmission();
}

void loop_accelerometer() {
    Adafruit_NeoPixel pixels(STRIP_LENGHT, LED_STRIP, NEO_GRB + NEO_KHZ800); //strip file to control it
    pixels.begin(); //initialize the strip
    // Read the X, Y, and Z values from the MMA8451Q accelerometer
    int16_t x = read16(MMA8451Q_OUT_X_MSB);
    int16_t y = read16(MMA8451Q_OUT_Y_MSB);
    int16_t z = read16(MMA8451Q_OUT_Z_MSB);

    // Print the X, Y, and Z values on the serial monitor
    Serial.print("X: ");
    Serial.print(x);
    Serial.print(", Y: ");
    Serial.print(y);
    Serial.print(", Z: ");
    Serial.println(z);

    pixels.clear();
    pixels.setPixelColor(6, pixels.Color(20, 20,20));
    if(x > 0){
        pixels.setPixelColor(1, pixels.Color(0, 150, 0));
        pixels.setPixelColor(2, pixels.Color(0, 150, 0));
    } else {
        pixels.setPixelColor(1, pixels.Color(0, 0, 150));
        pixels.setPixelColor(2, pixels.Color(0, 0, 150));
    }
    if(y > 0){
        pixels.setPixelColor(3, pixels.Color(0, 150, 0));
        pixels.setPixelColor(4, pixels.Color(0, 150, 0));
    } else {
        pixels.setPixelColor(3, pixels.Color(0, 0, 150));
        pixels.setPixelColor(4, pixels.Color(0, 0, 150));
    }
    if(z > 0){
        pixels.setPixelColor(5, pixels.Color(0, 150, 0));
        pixels.setPixelColor(6, pixels.Color(0, 150, 0));
    } else {
        pixels.setPixelColor(5, pixels.Color(0, 0, 150));
        pixels.setPixelColor(6, pixels.Color(0, 0,150));
    }
    if(x == y && y == z){
        //set all to zero
        for(int i = 0; i < STRIP_LENGHT; i++){
            pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        }
    }
    pixels.show();


    // Wait for 100 milliseconds
    delay(100);
}
#endif // __ACCELEROMETER__


// #include <Wire.h>        /*include Wire.h library*/
// void setup()
// {
//   Wire.begin();           /*Wire I2C communication START*/
//   Serial.begin(9600);    /*baud rate set for Serial Communication*/
//   while (!Serial);       /*Waiting for Serial output on Serial Monitor*/
//   Serial.println("\nI2C Scanner");
// }
// void loop()
// {
//   byte err, adr;       /*variable error is defined with address of I2C*/
//   int number_of_devices;
//   Serial.println("Scanning.");
//   number_of_devices = 0;
//   for (adr = 1; adr < 127; adr++ )
//   {
//     Wire.beginTransmission(adr);
//     err = Wire.endTransmission();

//     if (err == 0)
//     {
//       Serial.print("I2C device at address 0x");
//       if (adr < 16)
//         Serial.print("0");
//       Serial.print(adr, HEX);
//       Serial.println("  !");
//       number_of_devices++;
//     }
//     else if (err == 4)
//     {
//       Serial.print("Unknown error at address 0x");
//       if (adr < 16)
//         Serial.print("0");
//       Serial.println(adr, HEX);
//     }
//   }
//   if (number_of_devices == 0)
//     Serial.println("No I2C devices attached\n");
//   else
//     Serial.println("done\n");
//   delay(5000);             /*wait 5 seconds for the next I2C scan*/
// }