#ifndef __EEPROM_CONTROLLER__
#define __EEPROM_CONTROLLER__

// /* example */
// #include <Arduino.h>
// #include <Wire.h>
// #include <EEPROM.h>
// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "SPIFlash.h"

// #include <stdio.h>
// #include <stdlib.h>

// #include "pico/stdlib.h"
// #include "hardware/flash.h"

// #define FLASH_TARGET_OFFSET (256 * 1024)

// const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);

// void setup()
// {
//     pinMode(LED_BUILTIN, OUTPUT);
   
//     uint8_t random_data[FLASH_PAGE_SIZE];
//     for (int i = 0; i < FLASH_PAGE_SIZE; ++i)
//         random_data[i] = rand() >> 16;

//     flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
//     flash_range_program(FLASH_TARGET_OFFSET, random_data, FLASH_PAGE_SIZE);

//     bool mismatch = false;
//     for (int i = 0; i < FLASH_PAGE_SIZE; i++) {
//         if (random_data[i] != flash_target_contents[i])
//             mismatch = true;
//     }

//     if (mismatch) digitalWrite(LED_BUILTIN, LOW);
//     else   digitalWrite(LED_BUILTIN, HIGH);

// }

// void loop(){
// }
// // Define the I2C address of the EEPROM
// //512 pages of 64 bytes each
// #define EEPROM_ADDRESS 0x50
// //need a "partition table" to know where things are stored

// void setup_eeprom() {
//     // Start the serial communication
//     Serial.begin(9600);

//     // Start the I2C bus
//     Wire.begin();
// }

// void loop_eeprom() {
//     // // Write "Hello" to the EEPROM
//     // EEPROM.begin(32); // Initialize the EEPROM with a size of 32 bytes
//     // EEPROM.write(0, 'H');
//     // EEPROM.write(1, 'e');
//     // EEPROM.write(2, 'l');
//     // EEPROM.write(3, 'l');
//     // EEPROM.write(4, 'o');
//     // EEPROM.end(); // Commit the changes to the EEPROM

//     // // Wait for 1 second
//     // delay(1000);

//     // // Write "World" to the EEPROM
//     // EEPROM.begin(32); // Initialize the EEPROM with a size of 32 bytes
//     // EEPROM.write(5, 'W');
//     // EEPROM.write(6, 'o');
//     // EEPROM.write(7, 'r');
//     // EEPROM.write(8, 'l');
//     // EEPROM.write(9, 'd');
//     // EEPROM.end(); // Commit the changes to the EEPROM

//     // // Wait for 1 second
//     // delay(1000);

//     // // Read the values from the EEPROM
//     // EEPROM.begin(32); // Initialize the EEPROM with a size of 32 bytes
//     // char hello[6];
//     // char world[6];
//     // for (int i = 0; i < 5; i++) {
//     //     hello[i] = EEPROM.read(i);
//     //     world[i] = EEPROM.read(i + 5);
//     // }
//     // hello[5] = '\0';
//     // world[5] = '\0';
//     // EEPROM.end(); // Release the EEPROM

//     // // Print the values on the serial monitor
//     // Serial.print("Hello: ");
//     // Serial.println(hello);
//     // Serial.print("World: ");
//     // Serial.println(world);
// }

#endif // __EEPROM_CONTROLLER__
