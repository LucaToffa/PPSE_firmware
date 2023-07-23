#ifndef __WIFIMODULE__
#define __WIFIMODULE__

#include <Arduino.h>
#include "Defines.h"
#include <Wire.h>

//define at commands
#define RESTART "AT+RST"
#define DEEP_SLEEP "AT+GSLP0" //concatenate "=ms", time until wake up
#define SLEEP_MODE "AT+SLEEP" //"=0" "=1" "=2", DISABLE, LIGHT, MODEM
#define VERSION "AT+GMR" //get version info 
#define ECHO "ATE" //echo on/off "1" or "0"
#define GET_CURRENT_CONFIG "AT+UART_CUR?" 
#define GET_RAM "AT+SYSRAM?" //get free ram 

#define START_SERVER "AT+CIPSERVER=1,80" //start server on port 80, default is 333
#define STOP_SERVER "AT+CIPSERVER=0" //stop server
#define GET_LOCAL_IP "AT+CIFSR" //get ip address
#define LIST_NETWORKS "AT+CWLAP"
#define PING "AT+PING" // =<ip address>
#define CHECK_TIME "AT+CIPSNTPTIME?" //get time from ntp server

#define CLIENT_MODE "AT+CWMODE=0"
#define HOST_MODE "AT+CWMODE=1"
#define DUAL_MODE "AT+CWMODE=2"

#define CONNECT "AT+CWJAP=" // + "ssid" , "password" 
#define DISCONNECT "AT+CWQAP" //disconnect from network
#define CURRENT_NETWORK "AT+CWJAP?" //get current network

#define STATUS "AT+CIPSTATUS=?"

#define AT_CIPMUX "AT+CIPMUX=1"
#define AT_CIPSTART "AT+CIPSTART=0,\"TCP\",\""
#define AT_CIPSEND "AT+CIPSEND=0,"
#define AT_CIPCLOSE "AT+CIPCLOSE=0"
#define AT_CIPSTART "AT+CIPSTART=0,\"TCP\",\""
#define AT_CIPSEND "AT+CIPSEND=0,"
#define AT_CIPCLOSE "AT+CIPCLOSE=0"

void setup_wifi();
String Send_AT_Cmd(String command, const int timeout, boolean debug);
void setup_wifi_server();
void test_wifi_server();
void read_wifi_server();
  

#endif  //__WIFIMODULE__
