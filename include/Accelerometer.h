#ifndef __ACCELEROMETER__
#define __ACCELEROMETER__

#include <Arduino.h>
#include "Defines.h"

typedef enum {
  MMA8451_RANGE_8_G = 2, // +/- 8g
  MMA8451_RANGE_4_G = 1, // +/- 4g
  MMA8451_RANGE_2_G = 0  // +/- 2g (default value)
} mma8451_range_t;

typedef struct {
    int16_t rawx;
    int16_t rawy;
    int16_t rawz;
    float x;
    float y;
    float z;
} accelerometer_data_t;

//library not working with pico, doesnt even init the i2c successfully
void setRange(int range);
void setActivityState(int state);
int getRange();
void setup_accelerometer();
void raw_read_accelerometer();
float read_accelerometer(int axis);
void read_accelerometer();
void setAccelLeds();
void stop_accelerometer();


#endif // __ACCELEROMETER__

