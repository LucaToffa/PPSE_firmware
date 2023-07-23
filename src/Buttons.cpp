#include "Buttons.h"
#include <Debounce.h>

/*
To make buttons work as intended, let's switch to an interrupt-based approach.
Detect button press -> interrupt -> set flag -> read flag in main loop ->
reset flag after enough time has passed
[B1, B2, B3, B4] pressed state is stored in buttons_pressed[4]
reset on first use, don't accept new input until enough time has passed
*/

//debounced button objects
Debounce ButtonUp(BUTTON_UP, 80);
Debounce ButtonDown(BUTTON_DOWN, 80);
Debounce ButtonRight(BUTTON_RIGHT, 80);
Debounce ButtonLeft(BUTTON_LEFT, 80);
long int last_press = 0;
//button states
bool buttons_pressed[4] = {false, false, false, false};

void init_buttons(){
    //setup buttons
    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    //attach interrupts
    attachInterrupt(digitalPinToInterrupt(BUTTON_UP), button_interrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_DOWN), button_interrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_RIGHT), button_interrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_LEFT), button_interrupt, FALLING);
}

void reset_buttons(){
  buttons_pressed[0] = false;
    buttons_pressed[1] = false;
    buttons_pressed[2] = false;
    buttons_pressed[3] = false;
}

void set_button_states(bool up, bool down, bool right, bool left){
  buttons_pressed[0] = up;
  buttons_pressed[1] = down;
  buttons_pressed[2] = right;
  buttons_pressed[3] = left;
}

//function to call when button interrupt hits
void button_interrupt(){
  if(millis() - last_press > 50){
    buttons_pressed[0] = !digitalRead(BUTTON_UP);
    buttons_pressed[1] = !digitalRead(BUTTON_DOWN);
    buttons_pressed[2] = !digitalRead(BUTTON_RIGHT);
    buttons_pressed[3] = !digitalRead(BUTTON_LEFT);
    last_press = millis();
  }
  return;
}

int raw_read_buttons(){
    if(ButtonUp.read() == 1){
        return BUTTON_UP;
    }
    if(ButtonDown.read() == 1){
        return BUTTON_DOWN;
    }
    if(ButtonLeft.read() == 1){
        return BUTTON_LEFT;
    }
    if(ButtonRight.read() == 1){
        return BUTTON_RIGHT;
    }
    return 0;
}
