//GESTIONE DELLA SELEZIONE VERTICALE
#include "FSM_screen.h"
#include <Arduino.h>
#include "Defines.h"
#include "OLEDScreen.h"
#include "Buttons.h"
#include "Buzzer.h"
#include "Led.h"

extern bool buttons_pressed[4];

enum enum_state current_state = IDLE;
int current_btn = 0;

int position = 1;

//list of y/n states for the selection
extern bool selection_datasave[3]; //local wifi usb 
extern bool selection_sensors[3]; //gps acc temp
extern bool selection_others[3]; //wifi buzz leds

//funzione per disegnare la freccetta di selezione
void menu_selection(){
  if(buttons_pressed[0]){
    press_toneUD();
    if(position == 1){
      position = 3;
      draw_selection(current_state , position);
    }else{
      position--;
      draw_selection(current_state, position);
    }
  }
  else 
  if(buttons_pressed[1]){ 
    press_toneUD();
    if(position == 3){
      position = 1;
      draw_selection(current_state, position);
    }else{
      position++;
      draw_selection(current_state, position);
    }
  }
  current_btn = 0;

}   



//--------------------------------------------------------------------------------------------------------------------------------------------


//FUNZIONI DELLA MACCHINA A STATI MENU


enum_state fn_IDLE(){
  draw_screen(current_state);
  if(buttons_pressed[0] || buttons_pressed[1] || buttons_pressed[2] || buttons_pressed[3]){
    press_toneLR();
    current_state = HOME;
    draw_screen(current_state);
    draw_selection(current_state, position);
  }
  current_btn = 0;
  return current_state;
}

enum_state fn_HOME(){
  menu_selection();
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = IDLE;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  if(position == 1 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = DATA_RT;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
  } 
  if(position == 2 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = DATA_SAVE;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    draw_YN(current_state, 1);
    draw_YN(current_state, 2);
    draw_YN(current_state, 3);
  }
  if(position == 3 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = SETTINGS;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
  }
  current_btn = 0;
  return current_state;
}

enum_state fn_DATA_RT(){
  draw_screen(current_state); 
  draw_selection(current_state, position);
  menu_selection();
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = HOME;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  if(position == 1 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = GPS;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
  }
  if(position == 2 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = ACC;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
  }
  current_btn = 0;
  return current_state;
}

enum_state fn_GPS(){
  draw_screen(current_state); 
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = DATA_RT;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  current_btn = 0;
  return current_state;
}

enum_state fn_ACC(){
  draw_screen(current_state); 
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = DATA_RT;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  current_btn = 0;
  return current_state;
}

enum_state fn_DATA_SAVE(){               
  menu_selection();
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = HOME;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  if(position == 1 &&  buttons_pressed[2]){
    press_toneLR();
    selection_datasave[0] = !selection_datasave[0];
    draw_YN(current_state, position);
    //activate or deactivate local data saving
  }
  if(position == 2 &&  buttons_pressed[2]){
    press_toneLR();
    selection_datasave[1] = !selection_datasave[1];
    draw_YN(current_state, position);
    //activate or deactivate remote data transfer
  }
  if(position == 3 &&  buttons_pressed[2]){
    press_toneLR();
    selection_datasave[2] = !selection_datasave[2];
    draw_YN(current_state, position);
    //activate or deactivate wired data transfer
  }
  current_btn = 0;
  return current_state;
}

enum_state fn_SETTINGS(){
  menu_selection();
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = HOME;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  if(position == 1 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = SENSORS;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    draw_YN(current_state, 1);
    draw_YN(current_state, 2);
    draw_YN(current_state, 3);
  }
  if(position == 2 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = OTHERS;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    draw_YN(current_state, 1);
    draw_YN(current_state, 2);
    draw_YN(current_state, 3);
  }
  current_btn = 0;
  return current_state;
}

enum_state fn_SENSORS(){
  menu_selection();
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = SETTINGS;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  if(position == 1 &&  buttons_pressed[2]){
    press_toneLR();
    selection_sensors[0] = !selection_sensors[0];
    reset_led_strip();
    draw_YN(current_state, position);
    //activate or deactivate GPS
  }
  if(position == 2 &&  buttons_pressed[2]){
    press_toneLR();
    selection_sensors[1] = !selection_sensors[1];
    reset_led_strip();
    draw_YN(current_state, position);
    //activate or deactivate accelerometer
  }
  if(position == 3 &&  buttons_pressed[2]){
    press_toneLR();
    selection_sensors[2] = !selection_sensors[2];
    reset_led_strip();
    draw_YN(current_state, position);
    //activate or deactivate temperature sensor
  }
  current_btn = 0;
  return current_state;
}

enum_state fn_OTHERS(){
  menu_selection();
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = SETTINGS;
    position = 1;
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  if(position == 1 && buttons_pressed[2]){
    press_toneLR();
    selection_others[0] = !selection_others[0];
    draw_YN(current_state, position);
    //activate or deactivate wifi
  }
  if(position == 2 && buttons_pressed[2]){
    press_toneLR();
    selection_others[1] = !selection_others[1];
    draw_YN(current_state, position);
    //activate or deactivate buzzer
  }
  if(position == 3 && buttons_pressed[2]){
    press_toneLR();
    selection_others[2] = !selection_others[2];
    reset_led_strip();
    draw_YN(current_state, position);
    //activate or deactivate leds
  }
  current_btn = 0;
  return current_state;
}