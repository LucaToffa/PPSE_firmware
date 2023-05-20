#include "Buttons.h"
#include <Debounce.h>

//button objects
Debounce ButtonUp(BUTTON_UP, 50);
Debounce ButtonDown(BUTTON_DOWN, 50);
Debounce ButtonRight(BUTTON_RIGHT, 50);
Debounce ButtonLeft(BUTTON_LEFT, 50);

//button states
bool buttons_pressed[4] = {false, false, false, false};

void set_button_states(bool up, bool down, bool right, bool left){
  buttons_pressed[0] = up;
  buttons_pressed[1] = down;
  buttons_pressed[2] = right;
  buttons_pressed[3] = left;
}

int read_buttons(){
    if(ButtonUp.read() == HIGH){
        //Serial.println("Button 1 pressed");
        set_button_states(true, false, false, false);
        return BUTTON_UP;
    }
    if(ButtonDown.read() == HIGH){
        //Serial.println("Button 2 pressed");
        set_button_states(false, true, false, false);
        return BUTTON_DOWN;
    }
    if(ButtonLeft.read() == HIGH){
        //Serial.println("Button 3 pressed");
        set_button_states(false, false, false, true);
        return BUTTON_LEFT;
    }
    if(ButtonRight.read() == HIGH){
        //Serial.println("Button 4 pressed");
        set_button_states(false, false, true, false);
        return BUTTON_RIGHT;
    }
    set_button_states(false, false, false, false);
    return 0;
}