#ifndef __GPS__
#define __GPS__

#include <Arduino.h>

typedef struct Gps_struct {
    String raw_nmea = "";
    float latitude = 46.06; //default values for gps for late start
    float longitude = 11.15;
    float altitude = 0.0;
    float speed = 0.0;
    int direction = 0;
    uint8_t numSatellites = 0;
    //date and time
    int date = 0;
    int year = 2023;
    int month = 7;
    int day = 26;
    int time = 0;
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
int getUtcTime();
int getRawDate();
#endif // __GPS__


