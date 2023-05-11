#ifndef __BUZZER__
#define __BUZZER__

#include "Defines.h"
#include "StarWarsTheme.h"
//maybe use ezbuzzer library as basis to write this


void setup_buzzer();
void loop_buzzer();

void setup_buzzer();

void loop_buzzer();

void STT_buzzer();


// Define the frequency of each note
const int tone_G4 = 391;
const int tone_E4 = 329;
const int tone_C5 = 523;
const int tone_D5 = 587;
const int tone_E5 = 659;

// Define the duration of each note
const int QUARTER_NOTE = 500;
long int currentMillis = 0;
// Define the melody as an array of notes
const int melody[] = {
    tone_G4, tone_E4, tone_C5, tone_D5,
    tone_E5, tone_G4, tone_E4, tone_C5,
    tone_D5, tone_E5, tone_G4, tone_E4,
    tone_C5, tone_D5, tone_E5, 0 // the 0 denotes a pause in the melody
};

void setup_buzzer() {
    // Set the buzzer pin as an output
    pinMode(BUZZER, OUTPUT);
    currentMillis = millis();
}

void loop_buzzer() {
    // Play the melody
    static int i = 0; // Keep track of the note
    if(millis() - currentMillis >= QUARTER_NOTE) {
        currentMillis = millis();
        tone(BUZZER, melody[i], QUARTER_NOTE);
        i++;
        if( i == (sizeof(melody) / sizeof(int)) ){
            i = 0;
        }
    }

}

void STT_buzzer() {
    // Play the melody
    static int current_note = 0; // Keep track of the note
    if(millis() - currentMillis >= noteDuration) {
        currentMillis = millis();

        // calculates the duration of each note
        divider = melody[current_note + 1];
        if (divider > 0) {
            // regular note, just proceed
            noteDuration = (wholenote) / divider;
        } else if (divider < 0) {
            // dotted notes are represented with negative durations!!
            noteDuration = (wholenote) / abs(divider);
            noteDuration *= 1.5; // increases the duration in half for dotted notes
        }
        // we only play the note for 90% of the duration, leaving 10% as a pause
        tone(BUZZER, (STTmelody[current_note] / 2), noteDuration*0.9);
        current_note = current_note + 2;//increment by 2 to skip the duration
        if(current_note == notes*2){
            current_note = 0;
        }
    }

}

#endif

