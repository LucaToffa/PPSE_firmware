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
}; //in the original exit is its own choice
PAGES home = {
  "Main Menu", 
  {"", " 1. Real-time data", " 2. Communication", " 3. Settings", "", ""}
}; //demo??
//lasciando more dovremmo o saltare di 2, forse meglio toglierlo?
PAGES data_rt ={ 
  "Sensors", 
  //{"", " 1. GPS: ", " more", " 2. Acc: ", " more",  " 3. Temp: "}
  {"", " GPS: 46.06 11.15", " Acc: 0.0 0.0 0.0",  " Temp: ", "", ""} //temp sol? 
  //non c'è nulla di più duraturo di una soluzione temporanea
};
//add data to display in real time using += or %f ??
PAGES acc = {
  "Accelerometer", 
  {"", " x:0.0 ", " y:0.0", " z:0.0", " Mod:0.0", ""}
};
PAGES gps = {
  "GPS", 
  {"", " Lat:0.0 Lon:0.0", " h: 0.00 sats: 0", " time: 0:0:0", " date: 26/7/2023", ""}
}; 
//PAGES adc = {"ADC", {"1. Temperature\n", "2. Battery\n", "3. \n", "4. \n", "\n", "Exit"}};
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
//PAGES music = {"Music", {"1. Track1", "2. Track2", "3. Track3", "Pause", "Stop", "Exit"}};
//PAGES memory = {"Memory", {"1. Save", "2. Load", "3. Delete", "4. Format", "\n", "Exit"}};
//PAGES demo = {"Demo", {"1. Screen", "2. Leds", "3. Buzzer", "4. WiFi", "", "Exit"}};

//Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
//write and display are very slow, so only use when needed, never in a loop

// Define the figures to be displayed
// const uint8_t figures[5][8] = {
//     { 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00, 0x00 },  // Square
//     { 0x08, 0x1C, 0x2A, 0x08, 0x08, 0x08, 0x08, 0x00 },  // Arrow
//     { 0x18, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00 },  // House
//     { 0x1C, 0x22, 0x04, 0x08, 0x10, 0x22, 0x1C, 0x00 },  // Smiley face
//     { 0x1C, 0x22, 0x22, 0x1C, 0x08, 0x14, 0x22, 0x00 }   // Diamond
// };

void update_page(enum_state current_state){
  switch(current_state){
    case DATA_RT:
      pages[DATA_RT].Lines[1] = " 1.GPS: " + getLat() + " " + getLon();
      pages[DATA_RT].Lines[2] = " 2.Acc: " + String(read_accelerometer(0),1) + " " + String(read_accelerometer(1),1) + " " + String(read_accelerometer(2),1);
      pages[DATA_RT].Lines[3] = " 3.Temp: " + String(get_temp()) + " C";

      break;
    case GPS:
      pages[GPS].Lines[1] = " Lat:" + getLat() + " Lon:" + getLon();
      pages[GPS].Lines[2] = " h: " + getAlt() + " sats: " + getSats();;
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
  //draw single character
  //display.setTextSize(1);             // Normal 1:1 pixel scale
  //display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, (line+1)*8);             // Start at beginning of line, +1 to avoid the color bar
  //display.cp437(true);                        // Use full 256 char 'Code Page 437' font
  display.write(0x7E); //0x7E is the arrow
  display.display();
} 

//draws the entire page on screen
void draw_screen(enum_state state){
  display.clearDisplay();
  //display.setTextSize(1);             // Normal 1:1 pixel scale
  //display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(pages[state].Title); //draw title
  for(int i = 0; i < 6; i++){         //draw lines
    display.println(pages[state].Lines[i]);
  }
  display.print(pages[state].Change); //draw page arrows 
  display.display();
  //delay(2000);
} 

//changes a togglable selection
void refresh_YN(){

}
void draw_YN(enum_state state, int line){ //selection is index of selection[3]
  int selection = line - 1;
  //draw black rectangle only on the zone that needs to be updated
  display.fillRect(120, (line+1)*8, 6, 8, SSD1306_BLACK);
  //draw single character
  //display.setTextSize(1);             // Normal 1:1 pixel scale
  //display.setTextColor(SSD1306_WHITE);        // Draw white text
  // Start at end of line
  display.setCursor(120, (line+1)*8);            
  //display.cp437(true);                        // Use full 256 char 'Code Page 437' font
  //need a way to change the YN
  switch (state){
    case SENSORS:
      //selection_sensors[selection] = !selection_sensors[selection];
      if(selection_sensors[selection] == true)
        display.write('Y'); 
      else
        display.write('N');
      break;
    case OTHERS:
      //selection_others[selection] = !selection_others[selection];
      if(selection_others[selection] == true)
        display.write('Y'); 
      else
        display.write('N');
      break;
    case DATA_SAVE:
      //selection_datasave[selection] = !selection_datasave[selection];
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
  //delay(2000);
}


void init_display(){
    
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  //display.drawPixel(10, 10, SSD1306_WHITE);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
}

// void testdrawline() {
//   int16_t i;

//   display.clearDisplay(); // Clear display buffer

//   for(i=0; i<display.width(); i+=4) {
//     display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
//     display.display(); // Update screen with each newly-drawn line
//     delay(1);
//   }
//   for(i=0; i<display.height(); i+=4) {
//     display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   delay(250);

//   display.clearDisplay();

//   for(i=0; i<display.width(); i+=4) {
//     display.drawLine(0, display.height()-1, i, 0, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   for(i=display.height()-1; i>=0; i-=4) {
//     display.drawLine(0, display.height()-1, display.width()-1, i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   delay(250);

//   display.clearDisplay();

//   for(i=display.width()-1; i>=0; i-=4) {
//     display.drawLine(display.width()-1, display.height()-1, i, 0, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   for(i=display.height()-1; i>=0; i-=4) {
//     display.drawLine(display.width()-1, display.height()-1, 0, i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   delay(250);

//   display.clearDisplay();

//   for(i=0; i<display.height(); i+=4) {
//     display.drawLine(display.width()-1, 0, 0, i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }
//   for(i=0; i<display.width(); i+=4) {
//     display.drawLine(display.width()-1, 0, i, display.height()-1, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }

//   delay(2000); // Pause for 2 seconds
// }

// void testdrawrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2; i+=2) {
//     display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
//     display.display(); // Update screen with each newly-drawn rectangle
//     delay(1);
//   }

//   delay(2000);
// }

// void testfillrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2; i+=3) {
//     // The INVERSE color is used so rectangles alternate white/black
//     display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
//     display.display(); // Update screen with each newly-drawn rectangle
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawcircle(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
//     display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testfillcircle(void) {
//   display.clearDisplay();

//   for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
//     // The INVERSE color is used so circles alternate white/black
//     display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
//     display.display(); // Update screen with each newly-drawn circle
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawroundrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2-2; i+=2) {
//     display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
//       display.height()/4, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testfillroundrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2-2; i+=2) {
//     // The INVERSE color is used so round-rects alternate white/black
//     display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
//       display.height()/4, SSD1306_INVERSE);
//     display.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawtriangle(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
//     display.drawTriangle(
//       display.width()/2  , display.height()/2-i,
//       display.width()/2-i, display.height()/2+i,
//       display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
//     display.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testfilltriangle(void) {
//   display.clearDisplay();

//   for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
//     // The INVERSE color is used so triangles alternate white/black
//     display.fillTriangle(
//       display.width()/2  , display.height()/2-i,
//       display.width()/2-i, display.height()/2+i,
//       display.width()/2+i, display.height()/2+i, SSD1306_INVERSE);
//     display.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawchar(void) {
//   display.clearDisplay();

//   display.setTextSize(1);      // Normal 1:1 pixel scale
//   display.setTextColor(SSD1306_WHITE); // Draw white text
//   display.setCursor(0, 0);     // Start at top-left corner
//   display.cp437(true);         // Use full 256 char 'Code Page 437' font

//   // Not all the characters will fit on the display. This is normal.
//   // Library will draw what it can and the rest will be clipped.
//   for(int16_t i=0; i<256; i++) {
//     if(i == '\n') display.write(' ');
//     else          display.write(i);
//   }

//   display.display();
//   delay(2000);
// }

// void testdrawstyles(void) {
//   display.clearDisplay();

//   display.setTextSize(1);             // Normal 1:1 pixel scale
//   display.setTextColor(SSD1306_WHITE);        // Draw white text
//   display.setCursor(0,0);             // Start at top-left corner
//   display.println(F("Hello, world!"));

//   display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
//   display.println(3.141592);

//   display.setTextSize(2);             // Draw 2X-scale text
//   display.setTextColor(SSD1306_WHITE);
//   display.print(F("0x")); display.println(0xDEADBEEF, HEX);

//   display.display();
//   delay(2000);
// }

// void testscrolltext(void) {
//   display.clearDisplay();

//   display.setTextSize(2); // Draw 2X-scale text
//   display.setTextColor(SSD1306_WHITE);
//   display.setCursor(10, 0);
//   display.println(F("scroll"));
//   display.display();      // Show initial text
//   delay(100);

//   // Scroll in various directions, pausing in-between:
//   display.startscrollright(0x00, 0x0F);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
//   display.startscrollleft(0x00, 0x0F);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
//   display.startscrolldiagright(0x00, 0x07);
//   delay(2000);
//   display.startscrolldiagleft(0x00, 0x07);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
// }

// void testdrawbitmap(void) {
//   display.clearDisplay();

//   display.drawBitmap(
//     (display.width()  - LOGO_WIDTH ) / 2,
//     (display.height() - LOGO_HEIGHT) / 2,
//     logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
//   display.display();
//   delay(1000);
// }



// void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
//   int8_t f, icons[NUMFLAKES][3];

//   // Initialize 'snowflake' positions
//   for(f=0; f< NUMFLAKES; f++) {
//     icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
//     icons[f][YPOS]   = -LOGO_HEIGHT;
//     icons[f][DELTAY] = random(1, 6);
//     Serial.print(F("x: "));
//     Serial.print(icons[f][XPOS], DEC);
//     Serial.print(F(" y: "));
//     Serial.print(icons[f][YPOS], DEC);
//     Serial.print(F(" dy: "));
//     Serial.println(icons[f][DELTAY], DEC);
//   }

//   for(;;) { // Loop forever...
//     display.clearDisplay(); // Clear the display buffer

//     // Draw each snowflake:
//     for(f=0; f< NUMFLAKES; f++) {
//       display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
//     }

//     display.display(); // Show the display buffer on the screen
//     delay(200);        // Pause for 1/10 second

//     // Then update coordinates of each flake...
//     for(f=0; f< NUMFLAKES; f++) {
//       icons[f][YPOS] += icons[f][DELTAY];
//       // If snowflake is off the bottom of the screen...
//       if (icons[f][YPOS] >= display.height()) {
//         // Reinitialize to a random position, just off the top
//         icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
//         icons[f][YPOS]   = -LOGO_HEIGHT;
//         icons[f][DELTAY] = random(1, 6);
//       }
//     }
//   }
// }