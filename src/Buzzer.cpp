#include "Buzzer.h"
#include <Arduino.h>
//#include "StarWarsTheme.h"


// Define the duration of each note
const int QUARTER_NOTE = 500;
long int currentMillis = 0;

int tempo = 50;
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int STTmelody[] = {
  
  // Dart Vader theme (Imperial March) - Star wars 
  // Score available at https://musescore.com/user/202909/scores/1141521
  // The tenor saxophone part was used
  
  NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,//1
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,8, NOTE_C5,8, NOTE_C5,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4, //8  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,-8, NOTE_C5,16, 
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C5,-8, NOTE_C5,16,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  
  NOTE_C6,-8, NOTE_G5,16, NOTE_G5,2, REST,8, NOTE_C5,8,//13
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C6,-8, NOTE_C6,16,
  NOTE_F6,4, NOTE_DS6,8, NOTE_CS6,4, NOTE_C6,8, NOTE_AS5,4, NOTE_GS5,8, NOTE_G5,4, NOTE_F5,8,
  NOTE_C6,1
  
};

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
    notes = sizeof(STTmelody) / sizeof(STTmelody[0]) / 2;
}

void loop_buzzer() {
    // Play the melody
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


void STT_buzzer() {
    // Play the melody
    static int current_note = 0; // Keep track of the note
    if(millis() - currentMillis >= noteDuration) {
        currentMillis = millis();

        // calculates the duration of each note
        divider = STTmelody[current_note + 1];
        if (divider > 0) {
            // regular note, just proceed
            noteDuration = (wholenote) / divider;
        } else if (divider < 0) {
            // dotted notes are represented with negative durations!!
            noteDuration = (wholenote) / abs(divider);
            noteDuration *= 1.5; // increases the duration in half for dotted notes
        }
        // we only play the note for 90% of the duration, leaving 10% as a pause
        tone(BUZZER, ((STTmelody[current_note] / 2)/ 2), noteDuration*0.9);
        current_note = current_note + 2;//increment by 2 to skip the duration
        if(current_note == notes*2){
            current_note = 0;
        }
    }

}