#include "Buzzer.h"
#include <Arduino.h>
//#include "StarWarsTheme.h"

extern bool selection_datasave[3]; //local wifi usb 
extern bool selection_sensors[3]; //gps acc temp
extern bool selection_others[3]; //wifi buzz leds

// Define the duration of each note
const int QUARTER_NOTE = 500;
long int currentMillis = 0;

int alarm_notes[] = {749, 894, 662};

// Define the melody as an array of notes
const int Basemelody[] = {
    NOTE_G4, NOTE_E4, NOTE_C5, NOTE_D5,
    NOTE_E5, NOTE_G4, NOTE_E4, NOTE_C5,
    NOTE_D5, NOTE_E5, NOTE_G4, NOTE_E4,
    NOTE_C5, NOTE_D5, NOTE_E5, 0 // the 0 denotes a pause in the melody
};
void setup_buzzer() {
    // Set the buzzer pin as an output
    pinMode(BUZZER, OUTPUT);
    currentMillis = millis();
}

void loop_buzzer() {
    // Play the test melody
    static int i = 0; // Keep track of the note
    if(millis() - currentMillis >= QUARTER_NOTE) {
        currentMillis = millis();
        tone(BUZZER, Basemelody[i], QUARTER_NOTE);
        i++;
        if( i == (sizeof(Basemelody) / sizeof(int)) ){
            i = 0;
        }
    }

}

//tone for button presses
void press_toneUD(){
  if(selection_others[1]) tone(BUZZER, NOTE_D5, 100);
}

void press_toneLR(){
  if(selection_others[1]) tone(BUZZER, NOTE_A4, 100);
}

void alarm_tone() {
	if(selection_others[1]){ //
		for (int i = 0; i < 3; i++) {
			tone(BUZZER, alarm_notes[i], 380); 
			delay(400);
		}
	}
}
