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
//draw_selection(current_state, position);
  //current_btn = raw_read_buttons();     
  //if(BUTTON_UP == current_btn)
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
  else //if(BUTTON_DOWN == current_btn)
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
//delay(1);
//reset_buttons();

}   



//--------------------------------------------------------------------------------------------------------------------------------------------


//FUNZIONI DELLA MACCHINA A STATI MENU


enum_state fn_IDLE(){
  //funzione non ancora esistente per disegnare la schermata
  draw_screen(current_state);
  //draw_selection(current_state, position); 
  //current_btn = raw_read_buttons();                   
  //if(current_btn != 0)
  if(buttons_pressed[0] || buttons_pressed[1] || buttons_pressed[2] || buttons_pressed[3]){
    press_toneLR();
    current_state = HOME;
    Serial.println("HOME" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
  }
  current_btn = 0;
  //delay(1);
  //reset_buttons();
  return current_state;
}

enum_state fn_HOME(){
  //draw_screen(current_state); //avoid infinite redraw
  menu_selection();
  //current_btn = raw_read_buttons(); 
  //if(BUTTON_LEFT == current_btn)
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = IDLE;
    position = 1;
    Serial.println("IDLE" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }   
  //if(position == 1 && BUTTON_RIGHT == current_btn)
  if(position == 1 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = DATA_RT;
    position = 1;
    Serial.println("DATA_RT" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
  } 
  //if(position == 2 && BUTTON_RIGHT == current_btn)
  if(position == 2 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = DATA_SAVE;
    position = 1;
    Serial.println("DATA_SAVE" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    draw_YN(current_state, 1);
    draw_YN(current_state, 2);
    draw_YN(current_state, 3);
  }
  //if(position == 3 && BUTTON_RIGHT == current_btn)
  if(position == 3 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = SETTINGS;
    position = 1;
    Serial.println("SETTINGS" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
  }
  // else{
  //   current_state = HOME;
  // }
  current_btn = 0;
  //delay(1);
  //reset_buttons();
  return current_state;
}

enum_state fn_DATA_RT(){
  draw_screen(current_state); //cannot avoid infinite redraw, data are always changing
  draw_selection(current_state, position);
  menu_selection();
  //current_btn = raw_read_buttons();    
  //if(BUTTON_LEFT == current_btn)
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = HOME;
    position = 1;
    Serial.println("HOME" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  //if(position == 1 && BUTTON_RIGHT == current_btn)
  if(position == 1 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = GPS;
    position = 1;
    Serial.println("GPS" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
  }
  //if(position == 2 && BUTTON_RIGHT == current_btn)
  if(position == 2 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = ACC;
    position = 1;
    Serial.println("ACC" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
  }
  // else{
  //   current_state = DATA_RT;
  // }
  current_btn = 0;
  //delay(1);
  //reset_buttons();
  return current_state;
}

enum_state fn_GPS(){
  draw_screen(current_state); //cannot avoid infinite redraw, data are always changing
  //draw_selection(current_state, position);
  //current_btn = raw_read_buttons();    
  //if(BUTTON_LEFT == current_btn)
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = DATA_RT;
    position = 1;
    Serial.println("DATA_RT" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  // else{
  //   current_state = GPS;
  // }
  current_btn = 0;
  //delay(1);
  //reset_buttons();
  return current_state;
}

enum_state fn_ACC(){
  draw_screen(current_state); //cannot avoid infinite redraw, data are always changing
  //draw_selection(current_state, position);
  //current_btn = raw_read_buttons();    
  //if(BUTTON_LEFT == current_btn)
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = DATA_RT;
    position = 1;
    Serial.println("DATA_RT" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  // else{
  //   current_state = ACC;
  // }
  current_btn = 0;
  //delay(1);
  //reset_buttons();
  return current_state;
}

enum_state fn_DATA_SAVE(){
  //draw_screen(current_state);
  //current_btn = raw_read_buttons();    
  //Funzione non ancora esistente per scrivere Yes/No accanto alle diverse opzioni del menu
  //draw_YN(current_state);                
  menu_selection();
  //if(BUTTON_LEFT == current_btn)
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = HOME;
    position = 1;
    Serial.println("HOME" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  //if(position == 1 and BUTTON_RIGHT == current_btn)
  if(position == 1 &&  buttons_pressed[2]){
    press_toneLR();
    selection_datasave[0] = !selection_datasave[0];
    draw_YN(current_state, position);
    //activate or deactivate local data saving
  }
  //if(position == 2 and BUTTON_RIGHT == current_btn)
  if(position == 2 &&  buttons_pressed[2]){
    press_toneLR();
    selection_datasave[1] = !selection_datasave[1];
    draw_YN(current_state, position);
    //activate or deactivate remote data transfer
  }
  //if(position == 3 and BUTTON_RIGHT == current_btn)
  if(position == 3 &&  buttons_pressed[2]){
    press_toneLR();
    selection_datasave[2] = !selection_datasave[2];
    draw_YN(current_state, position);
    //activate or deactivate wired data transfer
  }
  // else{
  //   current_state = DATA_SAVE;
  // }
  current_btn = 0;
  //delay(1);
  //reset_buttons();
  return current_state;
}

enum_state fn_SETTINGS(){
  //draw_screen(current_state);
  menu_selection();
  //current_btn = raw_read_buttons();    
  //if(BUTTON_LEFT == current_btn)
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = HOME;
    position = 1;
    Serial.println("HOME" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  //if(position == 1 && BUTTON_RIGHT == current_btn)
  if(position == 1 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = SENSORS;
    position = 1;
    Serial.println("SENSORS" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    draw_YN(current_state, 1);
    draw_YN(current_state, 2);
    draw_YN(current_state, 3);
  }
  //if(position == 2 && BUTTON_RIGHT == current_btn)
  if(position == 2 &&  buttons_pressed[2]){
    press_toneLR();
    current_state = OTHERS;
    position = 1;
    Serial.println("OTHERS" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    draw_YN(current_state, 1);
    draw_YN(current_state, 2);
    draw_YN(current_state, 3);
  }
  // else{
  //   current_state = SETTINGS;
  // }
  current_btn = 0;
  //delay(1);
  //reset_buttons();
  return current_state;
}

enum_state fn_SENSORS(){
  //draw_screen(current_state);
  //draw_YN(current_state);
  menu_selection();
  //current_btn = raw_read_buttons();    
  //if(BUTTON_LEFT == current_btn)
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = SETTINGS;
    position = 1;
    Serial.println("SETTINGS" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  //if(position == 1 and BUTTON_RIGHT == current_btn)
  if(position == 1 &&  buttons_pressed[2]){
    press_toneLR();
    selection_sensors[0] = !selection_sensors[0];
    reset_led_strip();
    draw_YN(current_state, position);
    //activate or deactivate GPS
  }
  //if(position == 2 and BUTTON_RIGHT == current_btn)
  if(position == 2 &&  buttons_pressed[2]){
    press_toneLR();
    selection_sensors[1] = !selection_sensors[1];
    reset_led_strip();
    draw_YN(current_state, position);
    //activate or deactivate accelerometer
  }
  //if(position == 3 and BUTTON_RIGHT == current_btn)
  if(position == 3 &&  buttons_pressed[2]){
    press_toneLR();
    selection_sensors[2] = !selection_sensors[2];
    reset_led_strip();
    draw_YN(current_state, position);
    //activate or deactivate temperature sensor
  }
  // else{
  //   current_state = SENSORS;
  // }
  current_btn = 0;
  //delay(1);
  //reset_buttons();
  return current_state;
}

enum_state fn_OTHERS(){
  //draw_screen(current_state);
  //draw_YN(current_state);
  menu_selection();
  //current_btn = raw_read_buttons();    
  //if(BUTTON_LEFT == current_btn)
  if(buttons_pressed[3]){
    press_toneLR();
    current_state = SETTINGS;
    position = 1;
    Serial.println("SETTINGS" + String(current_state));
    draw_screen(current_state);
    draw_selection(current_state, position);
    return current_state;
  }
  //if(position == 1 and BUTTON_RIGHT == current_btn)
  if(position == 1 && buttons_pressed[2]){
    press_toneLR();
    selection_others[0] = !selection_others[0];
    draw_YN(current_state, position);
    //activate or deactivate wifi
  }
  //if(position == 2 and BUTTON_RIGHT == current_btn)
  if(position == 2 && buttons_pressed[2]){
    press_toneLR();
    selection_others[1] = !selection_others[1];
    draw_YN(current_state, position);
    //activate or deactivate buzzer
  }
  //if(position == 3 and BUTTON_RIGHT == current_btn)
  if(position == 3 && buttons_pressed[2]){
    press_toneLR();
    selection_others[2] = !selection_others[2];
    reset_led_strip();
    draw_YN(current_state, position);
    //activate or deactivate leds
  }
  // else{
  //   current_state = OTHERS;
  // }
  current_btn = 0;
  //delay(1);
  //reset_buttons();
  return current_state;
}