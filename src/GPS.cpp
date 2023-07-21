#include "GPS.h"
#include <Arduino.h>
#include "Defines.h"
#include "LED.h"


extern bool selection_datasave[3]; //local wifi usb 
extern bool selection_sensors[3]; //gps acc temp
extern bool selection_others[3]; //wifi buzz leds

// Initialize the GPS object
//SFE_UBLOX_GNSS myGPS;
UART gpsSerial = UART(GPS_RX, GPS_TX);
Gps_struct myGPS;
void setup_gps() {
  Serial.println("Initializing GPS module...");
  // Start the serial communication
  digitalWrite(GPS_EN, LOW); //let current through GPS
  digitalWrite(GPS_RST, LOW);//reset GPS
  delay(5);
  digitalWrite(GPS_RST, HIGH);
  delay(5);
  Serial.println("GPS resetted");
  gpsSerial.begin(9600); // Initialize the GPS module
  while(!gpsSerial){
    //Serial.println("Waiting for GPS serial...");
    if(selection_others[2] == true){
      led_strip_set_all(50, 0, 0);
    }
    
  }
  delay(1000);
  if(selection_others[2] == true){
    led_strip_set_all(0, 155, 0);
  }
  Serial.println("GPS module initialized!");
  delay(1000); // Give the GPS module some time to boot up

  gpsSerial.println("$PMTK314,-1*04"); //enable all NMEA sentences
  gpsSerial.println("$PMTK220,100*2F"); // Set the update rate to 10 Hz
  //gpsSerial.println("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"); //enable only GGA and RMC
  // // Initialize the GPS
  // if (myGPS.begin(Serial1) == false) {
  //     Serial.println("GPS not detected. Check wiring.");
  //     //while (1);
  // }

  // // Set the update rate to 1 Hz
  // myGPS.setNavigationFrequency(1);
  
  // Serial.println("GPS started");
  reset_led_strip();
}

//sprintf(buf1, "%02d:%02d:%02d %02d/%02d/%02d", GPS.hour, GPS.minute, GPS.second, GPS.day, GPS.month, GPS.year);

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
  Serial.println("GPS stopped");
  gpsSerial.end();
  reset_led_strip();  
}
/*

std::string s = "scott>=tiger>=mushroom";
std::string delimiter = ">=";

size_t pos = 0;
std::string token;
while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    std::cout << token << std::endl;
    s.erase(0, pos + delimiter.length());
}
std::cout << s << std::endl;


Output:
scott
tiger
mushroom

*/

/*
void loop() {
  if (gpsSerial.available()) {
    String nmeaMessage = gpsSerial.readStringUntil('\n');
    if (nmeaMessage.startsWith("$GPGGA")) {
      // Parse GGA message
      String tokens[15];
      int i = 0;
      for (char *p = strtok((char *)nmeaMessage.c_str(), ","); p != NULL; p = strtok(NULL, ",")) {
        tokens[i++] = String(p);
      }
      if (i == 15) {
        // Extract latitude and longitude
        String latitude = tokens[2];
        String longitude = tokens[4];
        Serial.print("Latitude: ");
        Serial.println(latitude);
        Serial.print("Longitude: ");
        Serial.println(longitude);
      }
    } else if (nmeaMessage.startsWith("$GPRMC")) {
      // Parse RMC message
      // Extract speed and course
    }
  }
}
*/

// float FixPosition(float Position)
// {
// 	float Minutes, Seconds;
	
// 	Position = Position / 100;
	
// 	Minutes = trunc(Position);
// 	Seconds = fmod(Position, 1);

// 	return Minutes + Seconds * 5 / 3;
// }

//base new parser, to modify
void parseGpsData() {
  char* buffer = const_cast<char*> (myGPS.raw_nmea.c_str());
  char *token;
  char *tokens[15];
  int tokenIndex = 0;

  // Split the sentence into tokens using strtok()
  token = strtok(buffer, ",");
  while (token != NULL && tokenIndex < 15) {
    tokens[tokenIndex++] = token;
    token = strtok(NULL, ",");
  }

  // Check the sentence type and number of tokens
  if (strcmp(tokens[0], "$GNGGA") == 0 && tokenIndex == 15) {
    // Parse GGA sentence
    float latitude = atof(tokens[2]);
    float longitude = atof(tokens[4]);
    char ns = tokens[3][0];
    char ew = tokens[5][0];
    float altitude = atof(tokens[9]);
    Serial.print("GGA - ");
    Serial.print("Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
    Serial.print("Altitude: ");
    Serial.println(altitude);
    myGPS.altitude = altitude;
  } else if (strcmp(tokens[0], "$GNRMC") == 0 && tokenIndex == 13) {
    // Parse RMC sentence
    float latitude = atof(tokens[3]);
    float longitude = atof(tokens[5]);
    char ns = tokens[4][0];
    char ew = tokens[6][0];
    float speed = atof(tokens[7]);
    float course = atof(tokens[8]);
    // Serial.print("RMC - ");
    // Serial.print("Latitude: ");
    // Serial.println(latitude, 6);
    // Serial.print("Longitude: ");
    // Serial.println(longitude, 6);
    // Serial.print("Speed: ");
    // Serial.println(speed);
    // Serial.print("Course: ");
    // Serial.println(course);

    myGPS.latitude = /*FixPosition*/(latitude/100);
		if (ns == 'S') myGPS.latitude = -myGPS.latitude;
		myGPS.longitude = /*FixPosition*/(longitude/100);
	  if (ew == 'W') myGPS.longitude = -myGPS.longitude;
		
  }
  else{
    Serial.println("Unknown Nmea");
  }
}

/*
void parseGpsData(){ //da modificare per il nostro caso
  char* Buffer = const_cast<char*> (myGPS.raw_nmea.c_str());
  float utc_time, latitude, longitude, HDOP, altitude, speed, course = 0.0;
  int Time = 0;
	int quality, satellites, date = 0;
	char active, ns, ew, units, speedstring[16], coursestring[16] = "";
	
  //if (GPSChecksumOK(Buffer, Count)){
		satellites = 0;
    Serial.println("Begin parsing");
    if(strlen(Buffer) < 42){
      return;
    }
		if (strncmp(Buffer+3, "GGA", 3) == 0){
			if (sscanf(Buffer+7, "%f,%f,%c,%f,%c,%d,%d,%f,%f,%c", &utc_time, &latitude, &ns, &longitude, &ew, &quality, &satellites, &HDOP, &altitude, &units) >= 8){
        Serial.println("into gga");
        // $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
        //add != 0 to the sscanf to check if the value is valid
				Time = utc_time;
				myGPS.hour = (int)(Time / 10000);
				myGPS.minute = (int)(Time / 100) % 100;
				myGPS.second = (int)Time % 100;
				//GPS.SecondsInDay = GPS.Hours * 3600 + GPS.Minutes * 60 + GPS.Seconds;					

				// if (GPS.UseHostPosition)
				// {
				// 	GPS.UseHostPosition--;
				// }
				//else if (satellites >= 4)
				//{
					myGPS.latitude = (latitude/100);
					if (ns == 'S') myGPS.latitude = -myGPS.latitude;
					myGPS.longitude = (longitude/100);
					if (ew == 'W') myGPS.longitude = -myGPS.longitude;
					myGPS.altitude = altitude;
				//}

				myGPS.numSatellites = satellites;  
        Serial.println("GGA parsed");   
			}
      Serial.println("Out of gga");
		}
		if (strncmp(Buffer+3, "RMC", 3) == 0){
			speedstring[0] = '\0';
			coursestring[0] = '\0';
			if (sscanf(Buffer+7, "%f,%c,%f,%c,%f,%c,%s,%s,%d", 
      &utc_time, &active, &latitude, &ns, &longitude, &ew, speedstring, coursestring, &date) >= 7){
        Serial.println("Into rmc");
				// $GPRMC,124943.00,A,5157.01557,N,00232.66381,W,0.039,,200314,,,A*6C

        myGPS.day = date / 10000;
        myGPS.month = ((date) / 100) % 100;
        myGPS.year = date % 100; 


				speed = atof(speedstring);
				course = atof(coursestring);
				
				myGPS.speed = speed;
				myGPS.direction = course;
			}
      Serial.println("RMC parsed");
		}
		// else if (strncmp(Buffer+3, "GSV", 3) == 0)
    //     {
    //         // Disable GSV
    //         printf("Disabling GSV\r\n");
    //         unsigned char setGSV[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39 };
    //         //SendUBX(setGSV, sizeof(setGSV)); //TODO
    //     }
		// else if (strncmp(Buffer+3, "GLL", 3) == 0)
    //     {
    //         // Disable GLL
    //         printf("Disabling GLL\r\n");
    //         unsigned char setGLL[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B };
    //         //SendUBX(setGLL, sizeof(setGLL));
    //     }
		// else if (strncmp(Buffer+3, "GSA", 3) == 0)
    //     {
    //         // Disable GSA
    //         printf("Disabling GSA\r\n");
    //         unsigned char setGSA[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32 };
    //         //SendUBX(setGSA, sizeof(setGSA));
    //     }
		// else if (strncmp(Buffer+3, "VTG", 3) == 0)
    //     {
    //         // Disable VTG
    //         printf("Disabling VTG\r\n");
    //         unsigned char setVTG[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x05, 0x47};
    //         //SendUBX(setVTG, sizeof(setVTG));
    //     }
    else
        {
            Serial.print("Unknown NMEA sentence:");
            Serial.println(Buffer);
        }
    //}
    //else
    //{
      // printf("Bad checksum\r\n");
	//}
  Serial.println("Out of parser");
}
  // $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
  //take each string up to the comma convert them and put them in the struct
  // char temp_string[256] = myGps.raw_nmea.c_str();
  // char value[10];
  // while(temp_string != '\n'){
  //   while(temp_string != ','){ // nono usa i delimiter
  //     //put chars until comma in a string
  //     value = temp_string;
  //     //if the string i empty, then the value is not valid

  //     if(value != ""){
  //       int i = myGps.second.toInt(); //each time should convert to the correct type, and to the correct variable
  //     }
  //   }
*/

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
      Serial.println("GPS data received!\n");
      Serial.println(raw_nmea);
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
