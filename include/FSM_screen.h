#ifndef FSM_SCREEN_H
#define FSM_SCREEN_H

enum enum_state{
  IDLE,
  HOME,
  DATA_RT,
  GPS,
  ACC,
  DATA_SAVE,
  SETTINGS,
  SENSORS,
  OTHERS
};


void menu_selection();
enum_state fn_IDLE();
enum_state fn_HOME();
enum_state fn_DATA_RT();
enum_state fn_GPS();
enum_state fn_ACC();
enum_state fn_DATA_SAVE();
enum_state fn_SETTINGS();
enum_state fn_SENSORS();
enum_state fn_OTHERS();

#endif