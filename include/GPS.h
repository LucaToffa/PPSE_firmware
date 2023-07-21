#ifndef __GPS__
#define __GPS__

#include <Arduino.h>

//#include <Wire.h>
//#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

//non funziona
typedef struct Gps_struct {
    String raw_nmea = "";
    float latitude = 0.0;
    float longitude = 0.0;
    float altitude = 0.0;
    float speed = 0.0;
    int direction = 0;
    uint8_t numSatellites = 0;
    //date and time
    int year = 2023;
    int month = 7;
    int day = 26;
    int hour = 0;
    int minute = 0;
    int second = 0;
} Gps_struct;

void setup_gps();
void loop_gps();
void parseGpsData();

String getLat();
String getLon();
String getAlt();
String getSats();
String getTime();
String getDate();

#endif // __GPS__

/*

// GPS TEST

#include <Arduino.h>

#define GPS_EN 18
#define GPS_RX 4
#define GPS_TX 5 // RX, TX pins for GPS module
#define GPS_RST 12
#define LED_ALIVE 20 

UART gpsSerial = UART(GPS_RX, GPS_TX);

void setup() { 
  delay(1000); // Give me time to bring up serial monitor
  pinMode(LED_ALIVE, OUTPUT); // Set the LED pin as output
  digitalWrite(LED_ALIVE, HIGH); // Turn on the LED 
  digitalWrite(GPS_EN, LOW); //let current through GPS
  //digitalWrite(GPS_RST, LOW); //reset GPS
  delay(1000);
  digitalWrite(GPS_RST, HIGH); //activate GPS

  Serial.begin(9600); // Initialize the serial monitor
  gpsSerial.begin(9600); // Initialize the GPS module
  delay(1000);
  Serial.println("Serial monitor initialized!");
  Serial.println("GPS module initialized!");
  delay(1000); // Give the GPS module some time to boot up

  gpsSerial.println("$PMTK314,-1*04"); //enable all NMEA sentences
  gpsSerial.println("$PMTK220,100*2F"); // Set the update rate to 10 Hz
}

void loop() {
  if (gpsSerial.available()) {
    char c = gpsSerial.read();
    Serial.print(c);
    if( c == '\n'){
      Serial.println("GPS data received!\n");
      digitalWrite(LED_ALIVE, !digitalRead(LED_ALIVE)); // Toggle the LED pin
      delay(1000); // Wait for 1000 milliseconds before reading again
    }
  }


  
}
*/

/*
$GNRMC,172624.00,A,4603.82067,N,01107.42463,E,5.611,133.56,120723,,,A*7A

$GNRMC: The message type and the GNSS constellation (in this case, GNSS).
172624.00: The UTC time of the position fix (17:26:24.00).
A: The status of the position fix (A means valid fix).
4603.82067,N: The latitude of the position in degrees and minutes (46 degrees, 3.82067 minutes North).
01107.42463,E: The longitude of the position in degrees and minutes (11 degrees, 7.42463 minutes East).
5.611: The speed over ground in knots.
133.56: The course over ground in degrees.
120723: The date of the position fix (12th July 2023).
: The magnetic variation is not provided.
: The magnetic variation direction is not provided.
A: The mode indicator (A = autonomous mode).

cold start teorico : 30s

*/

/*
Connect the Arduino to the ESP8266 module using a UART interface, such as SoftwareSerial or Serial1.

Initialize the UART interface on the Arduino and configure the baud rate to match the ESP8266 module.

Send the appropriate AT commands to the ESP8266 module to configure it as an access point. For example, you can send the following commands:

AT+CWMODE=2 : Set the ESP8266 module to AP mode.
AT+CWSAP="SSID","password" : Set the SSID and password for the access point.
AT+CIPAP="192.168.4.1" : Set the IP address for the access point.
Wait for the ESP8266 module to respond with an "OK" message to confirm that the commands were received and executed correctly.

The ESP8266 module should now be configured as an access point, and you can connect to it using a WiFi-enabled device and the SSID and password you set in step 3.

Configure the ESP8266 module as an access point using the AT commands, as I described in my previous response.
Connect to the ESP8266 module's AP using a WiFi-enabled device, and obtain the IP address of the module (which you set in step 3 of the AT command process).
Modify your Arduino code to connect to the ESP8266 module's IP address as a TCP client. This typically involves using the WiFiClient or EthernetClient library to establish a TCP connection to the module's IP address and port number.
Once the Arduino is connected to the ESP8266 module, you can exchange data between the two devices over the TCP connection. For example, you can send commands or data from the WiFi-enabled device to the Arduino through the ESP8266 module, and the Arduino can respond by sending data back over the TCP connection.

Create an HTML web page that includes any interactive elements you want to use, such as buttons or input fields.
Save the HTML file to the ESP8266 module's file system using the AT command AT+FSWRITE. This command allows you to write data to a file on the ESP8266 module's file system.
Configure the ESP8266 module to serve the web page using the AT command AT+CIPSERVER. This command sets up the module to act as a TCP server and serve the web page to clients that connect to its IP address and port number.
Modify your Arduino code to handle incoming requests from the ESP8266 module and respond appropriately. For example, you can use the WiFiServer or EthernetServer library to listen for incoming TCP connections and parse data received from the ESP8266 module.
When a button is pressed on the web page, use JavaScript to send an HTTP request to the ESP8266 module's IP address and port number, along with any data you want to send to the Arduino. The ESP8266 module will forward the request to the Arduino, which can respond with data that is sent back to the web page.
*/

