#include "Accelerometer.h"
#include "LED.h"
#include "Defines.h"
#include <Arduino.h>
#include <Wire.h>

//#include <Adafruit_NeoPixel.h>
//Adafruit_MMA8451 mma = Adafruit_MMA8451();
extern TwoWire myWire; 
//int16_t xyzRawAccel[3];
//float xyzDataAccel[3];
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
  //while(Wire.available() == 0); //never works
  int rangeSetting = myWire.read();
  return rangeSetting;
}

// // Read a single byte from address and return it as a byte
// byte readRegister(int I2CAddress, byte address)
// {
//   //Send a request
//   //Start talking to the device at the specified address
//   Wire.beginTransmission(I2CAddress);
//   //Send a bit asking for requested register address
//   Wire.write(address);
//   //Complete Transmission
//   Wire.endTransmission(false);
//   //Read the register from the device
//   //Request 1 Byte from the specified address
//   Wire.requestFrom(I2CAddress, 1);
//   //wait for response
//   while(Wire.available() == 0);
//   // Get the temp and read it into a variable
//   byte data = Wire.read();
//   return data;
// }

// // Writes a single byte (data) into address
// void writeRegister(int I2CAddress, unsigned char address, unsigned char data)
// {
//   //Send a request
//   //Start talking to the device at the specified address
//   Wire.beginTransmission(I2CAddress);
//   //Send a bit asking for requested register address
//   Wire.write(address);
//   Wire.write(data);
//   //Complete Transmission
//   Wire.endTransmission(false);
// }

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
    Serial.println("MMA8451 is active!\n");
    // Debug statements to check if the range was set correctly
    Serial.print("Range setting: ");
    Serial.println(getRange(), HEX);
	if(selection_others[2] == true){
		led_strip_set_all(0,155,0);
	}

	// // Turn on orientation config
	// myWire.beginTransmission(MMA8451_ADDRESS);
	// myWire.write(MMA8451_REG_PL_CFG);
  	// myWire.endTransmission(false);
	// myWire.requestFrom(MMA8451_ADDRESS, 1);
	// int a = myWire.read();
	// Serial.print("read_first orientation: ");
	// Serial.println(a);
    // //myWire.endTransmission(false);

	// myWire.beginTransmission(MMA8451_ADDRESS);
    // myWire.write(MMA8451_REG_PL_CFG);
	// myWire.write(0x40); //activation magic number
	// myWire.endTransmission(false);

	// myWire.beginTransmission(MMA8451_ADDRESS);
    // myWire.write(MMA8451_REG_PL_CFG);
  	// myWire.endTransmission(false);
	// myWire.requestFrom(MMA8451_ADDRESS, 1);
	// a = myWire.read();
	// Serial.print("read_back orientation: ");
	// Serial.println(a);
    // //myWire.endTransmission(false);

	// Serial.println("Printing Orientation:");
	// Serial.println(read_orientation());
    // delay(10000);

    // if(mma.begin(MMA8451Q_ADDR, &myWire)){
    //     Serial.println("MMA8451Q found!");
    //     //set leds to green
    //     for(int i=0; i < STRIP_LENGHT; i++) {
    //         led_strip_set_color(i, 0, 33, 0);
    //     }
    // }else{
    //     Serial.println("Could not start MMA8451Q!");
    //     //set leds to red
    //     for(int i=0; i < STRIP_LENGHT; i++) {
    //         led_strip_set_color(i, 33, 0, 0);
    //     }
    //     delay(1000);
    // }
    // mma.setRange(MMA8451_RANGE_8_G);
    // Serial.print("Range = "); Serial.print(mma.getRange());
    // Serial.println("G");
    reset_led_strip();
}

void stop_accelerometer(){
    setActivityState(0);
    //set leds to red
    led_strip_set_all(33, 0, 0);
}

void test_read_acc(){
    // uint8_t res = mma.readRegister8(MMA8451_REG_OUT_X_MSB);
    // Serial.println(res, HEX);
}

// String read_orientation(){
    // if(selection_sensors[1] == false){
    //     return "";
    // }

    // Serial.println("Orientation data: \n");
    // // Read the orientation register
    // myWire.beginTransmission(MMA8451_ADDRESS);
    // myWire.write(MMA8451_REG_PL_STATUS);
    // myWire.endTransmission(false);
    // myWire.requestFrom(MMA8451_ADDRESS, 1);
    // uint8_t pl = myWire.read() & 0x07;

// 	if(accel_data.x >= 0 && accel_data.y >= 0){
// 		Serial.println("Portrait Up Front");
// 		return "Up Front";
// 	}
// 	if(accel_data.x >= 0 && accel_data.y >= 0){
// 		Serial.println("Portrait Up Back");
// 		return "Up Back";
// 	}
// 	if(accel_data.x >= 0 && accel_data.y >= 0){
// 		Serial.println("Portrait Down Front");
// 		return "Down Front";
// 	}
// 	if(accel_data.x >= 0 && accel_data.y >= 0){
// 		Serial.println("Portrait Down Back");
// 		return "Down Back";
// 	}
// 	if(accel_data.x >= 0 && accel_data.y >= 0){
// 		Serial.println("Landscape Right Front");
// 		return "Right Front";
// 	}
// 	if(accel_data.x >= 0 && accel_data.y >= 0){
// 		Serial.println("Landscape Right Back");
// 		return "Right Back";
// 	}
// 	if(accel_data.x >= 0 && accel_data.y >= 0){	
// 		Serial.println("Landscape Left Front");
// 		return "Left Front";
// 	}
// 	if(accel_data.x >= 0 && accel_data.y >= 0){
// 		SSerial.println("Landscape Left Back");
// 		return "Left Back";
// 	}

      

// }

//for some reason this function is not working now
void raw_read_accelerometer(){
    Serial.println("Raw accelerometer data: \n");
    // Read the X, Y, and Z acceleration data
    myWire.beginTransmission(MMA8451_ADDRESS);
    myWire.write(MMA8451_OUT_X_MSB);
    myWire.endTransmission(false);
    myWire.requestFrom(MMA8451_ADDRESS, 6);
    
    accel_data.rawx = (myWire.read() << 8) | myWire.read();
    accel_data.rawy = (myWire.read() << 8) | myWire.read();
    accel_data.rawz = (myWire.read() << 8) | myWire.read();
    

    // mma.read();
    // Serial.print("X: "); Serial.print(mma.x); Serial.print("  ");
    // Serial.print("Y: "); Serial.print(mma.y); Serial.print("  ");
    // Serial.print("Z: "); Serial.print(mma.z); Serial.print("  ");
    // Serial.println("m/s^2 ");
    // xyzRawAccel[0] = mma.x; xyzRawAccel[1] = mma.y; xyzRawAccel[2] = mma.z;
    // accel_data.rawx = mma.x; accel_data.rawy = mma.y; accel_data.rawz = mma.z;
}

void read_accelerometer(){
    if(selection_sensors[1] == false){
        return;
    }

    Serial.println("Accelerometer data: \n");
    raw_read_accelerometer();
    // Convert the raw data to acceleration values
    accel_data.rawx >>= 2;
    accel_data.rawy >>= 2;
    accel_data.rawz >>= 2;
    //Serial.println("set buffer\n");
    uint16_t divider = 1;
    if (range == MMA8451_RANGE_8_G)
    divider = 1024;
    if (range == MMA8451_RANGE_4_G)
    divider = 2048;
    if (range == MMA8451_RANGE_2_G)
        divider = 4096;
    //Serial.println("set range\n");
    accel_data.x = (float)accel_data.rawx / divider;
    accel_data.y = (float)accel_data.rawy / divider;
    accel_data.z = (float)accel_data.rawz / divider;
    //Serial.println("set g\n");

    // Print the acceleration data to the serial monitor
    Serial.print("X acceleration: ");
    Serial.println(accel_data.x);
    Serial.print("Y acceleration: ");
    Serial.println(accel_data.y);
    Serial.print("Z acceleration: ");
    Serial.println(accel_data.z);    

    // mma.getEvent(&event);
    // Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    // Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    // Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
    // Serial.println("m/s^2 \n");
    // xyzDataAccel[0] = event.acceleration.x; xyzDataAccel[1] = event.acceleration.y; xyzDataAccel[2] = event.acceleration.z;
    // accel_data.x = event.acceleration.x; accel_data.y = event.acceleration.y; accel_data.z = event.acceleration.z;

}

//return the value of the accelerometer in the specified axis
float read_accelerometer(int axis){
    
    //mma.getEvent(&event);
    
    switch (axis)
    {
    case 0:
        return accel_data.x;
        break;
    case 1:
        return accel_data.y;
        break;
    case 2:
        return accel_data.z;
        break;
    case 3:
        //return acc vector magnitude
        return (sqrt(pow(accel_data.x, 2) 
                + pow(accel_data.y, 2) 
                + pow(accel_data.z, 2)));
    default:
        return nanf("Not a valid axis");
        break;
    }

    return 0;
}

//change color of the leds depending on the accelerometer data
void setAccelLeds(){
    //led_strip_set_color(0, 20, 20, 20);
    //read_accelerometer();
    if(selection_others[2] == false){
        return;
    }
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

    if(accel_data.x == accel_data.y  && accel_data.y  == accel_data.z){
        //set 0 and 7 to white
        led_strip_set_color(0, 50, 50, 50);
        led_strip_set_color(7, 50, 50, 50);
    }
}

void example_run(){
 // Read the 'raw' data in 14-bit counts
//   mma.read();
//   Serial.print("X:\t"); Serial.print(mma.x); 
//   Serial.print("\tY:\t"); Serial.print(mma.y); 
//   Serial.print("\tZ:\t"); Serial.print(mma.z); 
//   Serial.println();

//   /* Get a new sensor event */ 
//   sensors_event_t event; 
//   mma.getEvent(&event);

//   /* Display the results (acceleration is measured in m/s^2) */
//   Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
//   Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
//   Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
//   Serial.println("m/s^2 ");
  
  /* Get the orientation of the sensor */
//   uint8_t o = mma.getOrientation();
//   uint8_t o = mma.getRange();
  
//   switch (o) {
//     case MMA8451_PL_PUF: 
//       Serial.println("Portrait Up Front");
//       break;
//     case MMA8451_PL_PUB: 
//       Serial.println("Portrait Up Back");
//       break;    
//     case MMA8451_PL_PDF: 
//       Serial.println("Portrait Down Front");
//       break;
//     case MMA8451_PL_PDB: 
//       Serial.println("Portrait Down Back");
//       break;
//     case MMA8451_PL_LRF: 
//       Serial.println("Landscape Right Front");
//       break;
//     case MMA8451_PL_LRB: 
//       Serial.println("Landscape Right Back");
//       break;
//     case MMA8451_PL_LLF: 
//       Serial.println("Landscape Left Front");
//       break;
//     case MMA8451_PL_LLB: 
//       Serial.println("Landscape Left Back");
//       break;
//     }
//   Serial.println();
//   delay(500);
  

}