#include "GPS.h"
#include <Arduino.h>
#include "Defines.h"
#include "LED.h"


extern bool selection_datasave[3]; //local wifi usb 
extern bool selection_sensors[3]; //gps acc temp
extern bool selection_others[3]; //wifi buzz leds

//messages to stop unnedeed data
unsigned char setGSV[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39 };
unsigned char setGLL[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B };
unsigned char setGSA[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32 };
unsigned char setVTG[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x05, 0x47};
                       

// Initialize the GPS object
UART gpsSerial = UART(GPS_RX, GPS_TX);
Gps_struct myGPS;
void setup_gps() {
  // Start the serial communication
  digitalWrite(GPS_EN, LOW); //let current through GPS
  digitalWrite(GPS_RST, LOW);//reset GPS
  delay(5);
  digitalWrite(GPS_RST, HIGH);
  delay(5);
  gpsSerial.begin(9600); // Initialize the GPS module
  while(!gpsSerial){
    if(selection_others[2] == true){
      led_strip_set_all(50, 0, 0);
    }
    
  }
  delay(1000);
  if(selection_others[2] == true){
    led_strip_set_all(0, 155, 0);
  }
  delay(1000); // Give the GPS module some time to boot up

  //the set commands over serial
  gpsSerial.write(setGSV, sizeof(setGSV));
  gpsSerial.write(setGLL, sizeof(setGLL));
  gpsSerial.write(setGSA, sizeof(setGSA));
  gpsSerial.write(setVTG, sizeof(setVTG));

  reset_led_strip();
}

//turn off gps
void stop_gps(){
  if(selection_others[2] == true){
    led_strip_set_all(50, 0, 0);
  }
  digitalWrite(GPS_EN, HIGH); //stop current through GPS
  digitalWrite(GPS_RST, LOW);//reset GPS
  delay(5);
  digitalWrite(GPS_RST, HIGH);
  delay(5);
  //Serial.println("GPS stopped");
  gpsSerial.end();
  reset_led_strip();  
}

void parseGpsData() {
  char* buffer = const_cast<char*> (myGPS.raw_nmea.c_str());
  char *token;
  char *tokens[15];
  int tokenIndex = 0;

  // Split the sentence into tokens using strtok()
  while ((token = strsep(&buffer, ",")) != NULL){
    //printf("token=%s\n", token);
    if(tokenIndex == 15){
      break;
    }
    tokens[tokenIndex++] = token;
  }

  // Check the sentence type and number of tokens
  //$xxGGA,time,lat,NS,lon,EW,quality,numSV,HDOP,alt,altUnit,sep,sepUnit,diffAge,diffStation*cs
  if (strcmp(tokens[0], "$GNGGA") == 0) {
    // Parse GGA sentence
    if(tokens[1][0] !=  '\0'){
    float utc_time = atof(tokens[1]);
    myGPS.hour = ( (int)(utc_time / 10000) + TIME_SAVINGS + TIME_ZONE ) % 24; 
    myGPS.minute = (int)(utc_time / 100) % 100;
    myGPS.second = (int)utc_time % 100;
    }
    if(tokens[2][0] !=  '\0'){
      float latitude = atof(tokens[2]);
      myGPS.latitude = (latitude/100);
    }
    if(tokens[4][0] !=  '\0'){
      float longitude = atof(tokens[4]);
      myGPS.longitude = (longitude/100);
    }
    char ns = tokens[3][0];
    char ew = tokens[5][0];
    if (ns == 'S') myGPS.latitude = -myGPS.latitude;
    if (ew == 'W') myGPS.longitude = -myGPS.longitude;

    if(tokens[9][0] !=  '\0'){
      float altitude = atof(tokens[9]);
      myGPS.altitude = altitude;
    }

    myGPS.numSatellites = atoi(tokens[7]);
    
  //$xxRMC,time,status,lat,NS,lon,EW,spd,cog,date,mv,mvEW,posMode,navStatus*cs
  //$GNRMC,101145.00,A,4531.10253,N,01049.36655,E,1.485,,220723,,,A*69
  } else if (strcmp(tokens[0], "$GNRMC") == 0) {
    // Parse RMC sentence
    if(tokens[1][0] !=  '\0'){
      float utc_time = atof(tokens[1]);
      myGPS.time = utc_time;
      myGPS.hour = ( (int)(utc_time / 10000) + TIME_SAVINGS + TIME_ZONE ) % 24; 
      myGPS.minute = (int)(utc_time / 100) % 100;
      myGPS.second = (int)utc_time % 100;
    }
    if(tokens[3][0] !=  '\0'){
      float latitude = atof(tokens[3]);
      myGPS.latitude = (latitude/100);
    }
    if(tokens[5][0] !=  '\0'){
      float longitude = atof(tokens[5]);
      myGPS.longitude = (longitude/100);
    }
    char ns = tokens[4][0];
    char ew = tokens[6][0];
		if (ns == 'S') myGPS.latitude = -myGPS.latitude;
	  if (ew == 'W') myGPS.longitude = -myGPS.longitude;

    float speed = atof(tokens[7]);
    myGPS.speed = speed * 1.852; //convert knots to km/h
    
    if(tokens[9][0] !=  '\0'){
      int date = atoi(tokens[9]);
      myGPS.date = date;
      myGPS.day = date / 10000;
      myGPS.month = ((date) / 100) % 100;
      myGPS.year = date % 100; 
    }

  }
  else{
    //Serial.println("Unknown Nmea");
  }
}


void loop_gps() {
  // Update the GPS data
  String raw_nmea = "";
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    raw_nmea += c;
  
    //Serial.print(c);
    if(c == '$'){ //start of a new message
      raw_nmea = "";
      raw_nmea += c;
    }
    if( c == '\n'){
      raw_nmea += '\0';
      digitalWrite(LED_ALIVE, !digitalRead(LED_ALIVE)); // Toggle the LED pin
      //reset raw_nmea
      
      myGPS.raw_nmea = raw_nmea;
      return;
    }
  }
}

String getLat(){
  return String(myGPS.latitude);
}
String getLon(){
  return String(myGPS.longitude);
}
String getAlt(){
  return String(myGPS.altitude);
}
String getSats(){
  return String(myGPS.numSatellites);
}
String getTime(){
  return String(myGPS.hour) + ":" + String(myGPS.minute) + ":" + String(myGPS.second);
}
String getDate(){
  return String(myGPS.day) + "/" + String(myGPS.month) + "/" + String(myGPS.year);
}

int getUtcTime(){
  return myGPS.time;
}

int getRawDate(){
  return myGPS.date;
}