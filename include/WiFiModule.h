#ifndef __WIFIMODULE__
#define __WIFIMODULE__

#include <Arduino.h>
#include "Defines.h"
#include <Wire.h>

void setup_wifi();
void test_wifi();
String Send_AT_Cmd(String command, const int timeout, boolean debug);


  

#endif  //__WIFIMODULE__
