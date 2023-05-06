#ifndef __DEFINES__
#define __DEFINES__

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
#define UART_GPS_TX 4
#define UART_GPS_RX 5

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
#define BATTERY_VOLTAGE 28
#define TEMPERAURE 26

//constants
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C //oled file to control it
#define NUM_PAGES 11 //number of pages in the oled screen tbd
#define EEPROM_ADDRESS 0x50 //hopefully, already defined in the library
#define ACC_ADDRESS 0x00
#define STRIP_LENGHT 8

//add functin to scan i2c devices
//INCLUDES
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h> //find a way to make it work
#include "OLED_screen.h"

#include "Debounce.h"
#include <ezBuzzer.h>
#include "AT24C256.h"
//#include <WiFi.h> //which library for wifi over uart?

//GLOBAL VARIABLES
Adafruit_NeoPixel pixels(STRIP_LENGHT, LED_STRIP, NEO_GRB + NEO_KHZ800); //strip file to control it

Debounce ButtonUp(BUTTON_UP); //button class for combination readings, usable for every peripheral
Debounce ButtonDown(BUTTON_DOWN);
Debounce ButtonRight(BUTTON_RIGHT);
Debounce ButtonLeft(BUTTON_LEFT);


ezBuzzer Buzzer(BUZZER);

AT24C256 eeprom = AT24C256();

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

Peripheral_state Peripherals_state = PERIPHERAL_IDLE;

#include <stdio.h>
//#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"


/* 
int main() {
    stdio_init_all();
    printf("ADC Example, measuring GPIO26\n");

    adc_init();

    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);

    while (1) {
    // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = adc_read();
    printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);
    sleep_ms(500);
    }
} */

#endif
