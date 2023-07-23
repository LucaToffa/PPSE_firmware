#ifndef __BUTTONS__
#define __BUTTONS__

#include "Defines.h"
#include <Debounce.h>

#define BUTTON_UP_Idx 0
#define BUTTON_DOWN_Idx 1
#define BUTTON_RIGHT_Idx 2
#define BUTTON_LEFT_Idx 3

// void setup_buttons();
void init_buttons(); //setup buttons and interrupts
int raw_read_buttons();
void set_button_states(bool up, bool down, bool right, bool left);
void reset_buttons(); //reset the list of buttons pressed after use
void button_interrupt(); //function to call when button interrupt hits, sets the list of buttons pressed


#endif // __BUTTONS__