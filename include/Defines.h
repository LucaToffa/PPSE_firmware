#ifndef __DEFINES__
#define __DEFINES__

#include <Arduino.h>

//GPIO Pin mapping   
#define LED_ALIVE 20 
#define LED_RP 25
#define LED_STRIP 13

#define SDA 0 
#define SCL 1 

#define USB_ID 2
#define USB_OUT_EN 27
#define WIFI_TX 17
#define WIFI_RX 16
#define WIFI_EN 3
#define GPS_TX 5
#define GPS_RX 4

#define BUTTON_UP 22
#define BUTTON_DOWN 23
#define BUTTON_RIGHT 24
#define BUTTON_LEFT 25
#define BUZZER 21

#define GPS_EN 18
#define GPS_PPS 19
#define GPS_RST 12


#define ACC_SA0 10
#define ACC_INT1 14
#define ACC_INT2 15

#define SERVO3 7
#define SERVO1 8
#define SERVO2 9

#define EN_5VOLT 11
//ANALOG INPUTS (ADC)
#define BATTERY_VOLTAGE 28 //adc2 //unused
#define TEMPERAURE 26 //adc0

//constants
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C //oled file to control it
#define NUM_PAGES 9 //number of pages in the oled screen tbd
#define EEPROM_ADDRESS 0x50 //hopefully, already defined in the library
#define ACC_ADDRESS 0x00
#define STRIP_LENGHT 8

#define TIME_SAVINGS 1
#define TIME_ZONE 1

// Define the I2C address of the MMA8451Q accelerometer
#define MMA8451Q_ADDR   0x1C

// Define the addresses of the MMA8451Q registers
#define MMA8451_ADDRESS 0x1C // Address of the MMA8451 accelerometer
#define MMA8451_XYZ_DATA_CFG_REG 0x0E // Register address for XYZ_DATA_CFG
#define MMA8451_CTRL_REG1 0x2A // Register address for CTRL_REG1
#define MMA8451_OUT_X_MSB 0x01 // Register address for OUT_X_MSB
#define MMA8451_OUT_Y_MSB 0x03
#define MMA8451_OUT_Z_MSB 0x05
#define MMA8451_REG_PL_STATUS 0x10 //orientation read register
#define MMA8451_REG_PL_CFG 0x11 //to activate orientation register write 0x40 there


#define MMA8451_PL_PUF 0 //Portrait Up Front
#define MMA8451_PL_PUB 1 //Portrait Up Back
#define MMA8451_PL_PDF 2 //Portrait Down Front
#define MMA8451_PL_PDB 3 //Portrait Down Back
#define MMA8451_PL_LRF 4 //Landscape Right Front
#define MMA8451_PL_LRB 5 //Landscape Right Back
#define MMA8451_PL_LLF 6 //Landscape Left Front
#define MMA8451_PL_LLB 7 //Landscape Left Back


// Define the I2C address of the GPS

enum Peripheral_state{
    MAIN_MENU,
    WIFI_SENDING,
    WIFI_RECEIVING,
    GPS_SENDING,
    ACC_SENDING,
    LEDS_DATA,
    TEMPERATURE_DATA,
    BATTERY_DATA,
    BUZZER_SIGNAL,
    PERIPHERAL_IDLE,
    PERIPHERAL_ERROR
};

typedef struct Data_storage{
    float latitude;
    float longitude;
    float altitude;
    int utc_time;
    int date;

    float temp;

    float accx;
    float accy;
    float accz;
}Data_storage;

#define READING_TIME 5000 //time between reads of the sensors
#define FAST_SPEED 10 //speed up sensor that need frequent update
#define WRITING_TIME 5000 //time between writes of the sensors
#endif
