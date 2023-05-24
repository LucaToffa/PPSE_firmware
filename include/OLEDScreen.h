#ifndef __OLED_SCREEN__
#define __OLED_SCREEN__

#include "Defines.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h> 

#define SSD1306_BLACK 0   ///< Draw 'off' pixels
#define SSD1306_WHITE 1   ///< Draw 'on' pixels
#define SSD1306_INVERSE 2 ///< Invert pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

typedef struct PAGES{
    String Title;
    String Lines[6];
    String Change = " <-prev   next->";
}PAGES;

void draw_line(String line, int line_number = 0, int size = 1, int color = SSD1306_WHITE);
void init_display();
#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2
void testdrawline();
void testdrawrect(void);
void testfillrect(void);
void testdrawcircle(void);
void testfillcircle(void);
void testdrawroundrect(void);
void testfillroundrect(void);
void testdrawtriangle(void);
void testfilltriangle(void);
void testdrawchar(void);
void testdrawstyles(void);
void testscrolltext(void);
void testdrawbitmap(void);
void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h);

static const unsigned char PROGMEM logo_bmp[] = { 
  0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 
};

#endif  // __OLED_SCREEN__