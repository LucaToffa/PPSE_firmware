#ifndef __BUTTONS__
#define __BUTTONS__

#include "Defines.h"
#include <Debounce.h>

#define BUTTON_UP_Idx 0
#define BUTTON_DOWN_Idx 1
#define BUTTON_RIGHT_Idx 2
#define BUTTON_LEFT_Idx 3

void setup_buttons();
int read_buttons();
void set_button_states(bool up, bool down, bool right, bool left);
bool get_button_states(int button);


#endif // __BUTTONS__