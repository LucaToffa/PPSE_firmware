#include "Buttons.h"
#include <Debounce.h>

//button objects
Debounce ButtonUp(BUTTON_UP, 50);
Debounce ButtonDown(BUTTON_DOWN, 50);
Debounce ButtonRight(BUTTON_RIGHT, 50);
Debounce ButtonLeft(BUTTON_LEFT, 50);
long int last_press = 0;
//button states
bool buttons_pressed[4] = {false, false, false, false};

void set_button_states(bool up, bool down, bool right, bool left){
  buttons_pressed[0] = up;
  buttons_pressed[1] = down;
  buttons_pressed[2] = right;
  buttons_pressed[3] = left;
}

int read_buttons(){
    
    if(ButtonUp.count() == 1){
        //Serial.println("Button 1 pressed");
        set_button_states(true, false, false, false);
        ButtonUp.resetCount();
        return BUTTON_UP;
    }
    if(ButtonDown.count() == 1){
        //Serial.println("Button 2 pressed");
        set_button_states(false, true, false, false);
        ButtonDown.resetCount();
        return BUTTON_DOWN;
    }
    if(ButtonLeft.count() == 1){
        //Serial.println("Button 3 pressed");
        set_button_states(false, false, false, true);
        ButtonLeft.resetCount();
        return BUTTON_LEFT;
    }
    if(ButtonRight.count() == 1){
        //Serial.println("Button 4 pressed");
        set_button_states(false, false, true, false);
        ButtonRight.resetCount();
        return BUTTON_RIGHT;
    }
    set_button_states(false, false, false, false);
    return 0;
}