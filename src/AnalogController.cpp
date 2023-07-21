#include "AnalogController.h"
#include "Arduino.h"
// Define constants for temperature conversion
// const float VREF = 3.3;    // reference voltage of Pico
// const float ADC_RES = VREF / 65535.0;   // ADC resolution of Pico
// const float TC_SCALE = 100.0;   // Temperature scale factor

float last_temp = 0.0;

void example_adc(){
  Serial.begin(9600);
  delay(1000);
  Serial.println("ADC Example, measuring GPIO26\n");
  // Initialise ADC on a pin
  adc_init();
  // Make sure GPIO is high-impedance, no pullups etc
  adc_gpio_init(TEMPERAURE);
  // Select ADC input 0 (GPIO26) (0-3,ADC0-ADC3 = GPIO26-29 )
  adc_select_input(ADC0);

  while (1) {
    // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = analogRead(TEMPERAURE);//adc_read();
    Serial.print("Raw value: ");
    Serial.print(result, HEX);
    Serial.print(" voltage: ");
    Serial.println((result * conversion_factor - 0,5)/0,01);
    sleep_ms(500);
  }
}

void battery_read(){
  Serial.begin(9600);
  delay(1000);
  // Initialise ADC on a pin
  adc_init();
  // Make sure GPIO is high-impedance, no pullups etc
  adc_gpio_init(BATTERY_VOLTAGE);
  // Select ADC input 0 (GPIO26) (0-3,ADC0-ADC3 = GPIO26-29 )
  adc_select_input(2);
  const float conversion_factor = 3.3f / (1 << 12);
  while(true){
    uint16_t result = analogRead(BATTERY_VOLTAGE);//adc_read();
    Serial.print("Raw value: ");
    Serial.print(result, HEX);
    Serial.print(" voltage: ");
    Serial.println(result * conversion_factor);
    sleep_ms(500);
  }

}

void setup_temp(){
  // Initialise ADC on a pin
  adc_init();
  // Make sure GPIO is high-impedance, no pullups etc
  adc_gpio_init(TEMPERAURE);
  // Select ADC input 0 (GPIO26) (0-3,ADC0-ADC3 = GPIO26-29 )
  adc_select_input(ADC0); 
  adc_set_clkdiv(500);

}

float read_temp(){
  // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
  const float conversion_factor = 3.3f / (1 << 12);
  int16_t raw_result = adc_read(); //analogRead(TEMPERAURE);
  Serial.print("Raw value: ");
  Serial.print(raw_result, HEX);
  float result = ((float)raw_result * conversion_factor - 0.5)/0.01;
  Serial.print(" Temperature: ");
  Serial.println(result);
  last_temp = result;
  return result;
}

float get_temp(){
  return last_temp;
}