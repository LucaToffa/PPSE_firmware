#include "OLEDScreen.h"
#include "FSM_screen.h"
#include "String.h"
#include "Accelerometer.h"
#include "GPS.h"
#include "AnalogController.h"

extern TwoWire myWire;  
//list of y/n states for the selection
extern bool selection_datasave[3]; //local wifi usb 
extern bool selection_sensors[3]; //gps acc temp
extern bool selection_others[3]; //wifi buzz leds

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &myWire, OLED_RESET, 40000UL, 10000UL);

//per evitare che il primo elemento sia giallo serve una riga vuota all inizio
//tutto parte in pos 2 -> cambio nella funzione draw_screen, fsm rimane invariata 
PAGES idle = {
  " PPSE",
  {"", " UP & DOWN", " to move cursor", " LEFT to go back", " RIGHT to select", " Press any..."}
}; 
PAGES home = {
  "Main Menu", 
  {"", " 1. Real-time data", " 2. Communication", " 3. Settings", "", ""}
}; 
PAGES data_rt ={ 
  "Sensors", 
  {"", " GPS: 0.00 0.00", " Acc: 0.0 0.0 0.0",  " Temp: 0.0", "", ""} 
};
PAGES acc = {
  "Accelerometer", 
  {"", " x:0.0 ", " y:0.0", " z:0.0", " Mod:0.0", ""}
};
PAGES gps = {
  "GPS", 
  {"", " Lat:0.00 Lon:0.00", " AL: 0.00 sats: 0", " time: 0:0:0", " date: 1/1/2000", ""}
}; 
PAGES data_save = {
  "Communication", 
  {"", " 1. Local", " 2. Wifi", " 3. USB", "", ""}
};
PAGES settings = {
  "Settings", 
  {"", " Active sensors", " Active signals", " ", " ", " "}
};
PAGES sensors = {
  "(De)activate sensors", 
  {"", " 1.GPS ", " 2.Accelerometer", " 3.Temp", " ", " "}
};
PAGES others = {
  "(De)activate signals", 
  {"", " 1.Wi-fi", " 2.Buzzer", " 3.Leds", " ", " "}
};
//PAGES demo = {"Demo", {"1. Screen", "2. Leds", "3. Buzzer", "4. WiFi", "", "Exit"}};

PAGES pages[NUM_PAGES] = {
  idle,
  home,
  data_rt,
  gps,
  acc,
  data_save,
  settings,
  sensors,
  others
};


void update_page(enum_state current_state){
  switch(current_state){
    case DATA_RT:
      pages[DATA_RT].Lines[1] = " 1.GPS: " + getLat() + " " + getLon();
      pages[DATA_RT].Lines[2] = " 2.Acc: " + String(read_accelerometer(0),1) + " " + String(read_accelerometer(1),1) + " " + String(read_accelerometer(2),1);
      pages[DATA_RT].Lines[3] = " 3.Temp: " + String(get_temp()) + " C";

      break;
    case GPS:
      pages[GPS].Lines[1] = " Lat:" + getLat() + " Lon:" + getLon();
      pages[GPS].Lines[2] = " Al: " + getAlt() + " sats: " + getSats();;
      pages[GPS].Lines[3] = " time: " + getTime();
      pages[GPS].Lines[4] = " date: " + getDate();
      break;
    case ACC:
      pages[ACC].Lines[1] = " x: " + String(read_accelerometer(0));
      pages[ACC].Lines[2] = " y: " + String(read_accelerometer(1));
      pages[ACC].Lines[3] = " z: " + String(read_accelerometer(2)); //" Orien: " + read_orientation();
      pages[ACC].Lines[4] = " Mod: " + String(read_accelerometer(3));
      break;
    default:
      Serial.println("Error: update_page");
      break;
  }
}
//draws the selection arrow at the current position
void draw_selection(enum_state state, int line){
  //draw black rectangle
  display.fillRect(0, 8, 6, 50, SSD1306_BLACK);
  display.setCursor(0, (line+1)*8); // Start at beginning of line, +1 to avoid the color bar
  display.write(0x7E); //0x7E is the arrow (~)
  display.display();
} 

//draws the entire page on screen
void draw_screen(enum_state state){
  display.clearDisplay();
  display.setCursor(0,0);             // Start at top-left corner
  display.println(pages[state].Title); //draw title
  for(int i = 0; i < 6; i++){         //draw lines
    display.println(pages[state].Lines[i]);
  }
  display.print(pages[state].Change); //draw page arrows 
  display.display();
} 

void draw_YN(enum_state state, int line){ //selection is index of selection[3]
  int selection = line - 1;
  //draw black rectangle only on the zone that needs to be updated
  display.fillRect(120, (line+1)*8, 6, 8, SSD1306_BLACK);
  display.setCursor(120, (line+1)*8);            
  switch (state){
    case SENSORS:
      if(selection_sensors[selection] == true)
        display.write('Y'); 
      else
        display.write('N');
      break;
    case OTHERS:
      if(selection_others[selection] == true)
        display.write('Y'); 
      else
        display.write('N');
      break;
    case DATA_SAVE:
      if(selection_datasave[selection] == true)
        display.write('Y'); 
      else
        display.write('N');
      break;
    default:
      break;
  }
  display.display();

} 

//maximum char lenght is 21 at size 1, line_number 0 is the color bar
//21*8 lines = 168 chars
void draw_line(String line, int line_number, int size, int color){
  display.clearDisplay();
  display.setTextSize(size);                  // Normal 1:1 pixel scale
  display.setTextColor(color);                // Draw white text
  display.setCursor(1, line_number*8*size);   // Start at the beginning of the line +1
  display.cp437(true);                        // Use full 256 char 'Code Page 437' font
  display.write(line.c_str());
  display.display();
}

void init_display(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;); // Don't proceed if unable to connect, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);
  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(1);                // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);   // Draw white text
  display.display();
  delay(1000);
}

