#include "AnalogController.h"
#include "Arduino.h"
// Define constants for temperature conversion
const float VREF = 3.3;    // reference voltage of Pico
const float ADC_RES = VREF / 65535.0;   // ADC resolution of Pico
const float TC_SCALE = 100.0;   // Temperature scale factor

void example_adc(){
  Serial.begin(9600);
  delay(1000);
  Serial.println("ADC Example, measuring GPIO26\n");
  // Initialise ADC on a pin
  adc_init();
  // Make sure GPIO is high-impedance, no pullups etc
  adc_gpio_init(TEMPERAURE);
  // Select ADC input 0 (GPIO26) (0-3,ADC0-ADC3 = GPIO26-29 )
  adc_select_input(0);

  while (1) {
  // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
  const float conversion_factor = 3.3f / (1 << 12);
  uint16_t result = analogRead(TEMPERAURE);//adc_read();
  Serial.print("Raw value: ");
  Serial.print(result, HEX);
  Serial.print(" voltage: ");
  Serial.println(result * conversion_factor);
  sleep_ms(500);
  }
}

void setup_temp(){

}

void loop_temp(){

}