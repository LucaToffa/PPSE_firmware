#include "AnalogController.h"
#include "Arduino.h"

float last_temp = 0.0;

void example_adc(){
  Serial.begin(9600);
  delay(1000);
  Serial.println("ADC Example,\n");
  // Initialise ADC on a pin
  adc_init();
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


void setup_temp(){
  adc_init();
  adc_gpio_init(TEMPERAURE);
  // Select ADC input 0 (GPIO26) (0-3,ADC0-ADC3 = GPIO26-29 )
  adc_select_input(ADC0); 
  adc_set_clkdiv(500);

}

float read_temp(){
  // 12-bit conversion, where ADC_VREF == 3.3 V
  const float conversion_factor = 3.3f / (1 << 12);
  int16_t raw_val = adc_read();
  float result = ((float)raw_val * conversion_factor - 0.5)/0.01;
  last_temp = result;
  return result;
}

float get_temp(){
  return last_temp;
}