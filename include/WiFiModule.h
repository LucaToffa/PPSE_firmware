#ifndef __WIFIMODULE__
#define __WIFIMODULE__

#include "Arduino.h"

/*example*/

// #include <SoftwareSerial.h>    
// SoftwareSerial esp(6, 7);   //Pin 6 and 7 act as RX and TX. Connect them to TX and RX of ESP8266   
   
// #define DEBUG true
// int number;
// String mySSID = "WRITE_YOUR_SSID";  // Wi-Fi SSID
// String myPWD = "WRITE_YOUR_PASSWORD"; // Wi-Fi Password
// String myAPI = "R6YGBXOY5V******";   // WRITE API Key
// String myHOST = "api.thingspeak.com";
// String myPORT = "80";
// String myFIELD = "field1"; 

// void setup()
// {
//   Serial.begin(115200);
//   esp.begin(115200);
//   Send_AT_Cmd("AT+RST", 1000, DEBUG);                      
//   Send_AT_Cmd("AT+CWMODE=1", 1000, DEBUG);                 
//   Send_AT_Cmd("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   
//   delay(1000); 
// }

//   void loop()
//   {
//     number = random(100); // Send a random number between 1 and 100
//     String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(number);
//     Send_AT_Cmd("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
//     Send_AT_Cmd("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
//     Send_AT_Cmd("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
//     esp.find(">"); 
//     esp.println(sendData);
//     Serial.print("Value to be sent: ");
//     Serial.println(number);
//     Send_AT_Cmd("AT+CIPCLOSE=0",1000,DEBUG);
//     Serial.println("Done!");
//     Serial.println("");
//     delay(10000);
//   }

//   String Send_AT_Cmd(String command, const int timeout, boolean debug)
// {
//   Serial.print(command);
//   Serial.println("     ");
  
//   String response = "";
//   esp.println(command);
//   long int time = millis();
//   while ( (time + timeout) > millis())
//   {
//     while (esp.available())
//     {
//       char c = esp.read();
//       response += c;
//     }
//   }
//   if (debug)
//   {
//     //Serial.print(response);
//   }
//   return response;
// }
  

#endif  //__WIFIMODULE__