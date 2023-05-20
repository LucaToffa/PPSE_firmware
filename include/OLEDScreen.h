#ifndef __OLED_SCREEN__
#define __OLED_SCREEN__

#include "Defines.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h> 

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

typedef struct PAGES{
    String Title;
    String Lines[6];
    String Change = " <-prev   next->";
}PAGES;

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

#endif  // __OLED_SCREEN__