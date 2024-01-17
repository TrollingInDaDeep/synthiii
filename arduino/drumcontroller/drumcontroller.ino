// Midi Controller Prototype
// 1 Multiplexer on A0 reading 8 POTs
// Multiplexer CD4051 control Pins on 10,11,12 (A, B, C)
// Keypad on pins 2-9                                                                              
// 24.10.2021 by dri and se internet under bedingungen
// where he just refused to be a larry and just play games
// however he will battlefield 4 right afterwards
// ... if this shit here is working

#include <Keypad.h>
//#include <MIDIUSB.h>
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();
// Keypad

const byte ROWS = 4;
const byte COLS = 4;

int kpc = 144; // Channel 1 Note on
int midiC = 60; // Standard midi C
int transpose = 0;

char hexaKeys[ROWS][COLS] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', '10', '11'},
  {'12', '13', '14', '15'}
};

char drumPadChars [16] = {'1', '2', '3', '+', '4', '5', '6', '>', '7', '8', '9', '!', '*', '0', '#', 'R'};


// flachbandkabel:
// weisser leerer pin links
// R1, R2, C1, C2, C3, C4, R3, R4
byte rowPins[ROWS] = {7, 6, 8, 9}; 
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad kpd = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// drum definitions
const int numDrumSteps = 16; //number of notes / steps the drum sequencer has
int drumStepPointer = 0; // pointer at which step in the drum sequencer we are

bool seq = false; // if false -> keypad is drum player
                  // if true -> keypad is to enable/disable trigger of certain notes

//bools if a drum sound should be triggered at the selected step
int kickNote = 60; // note to be triggered when kick is sent
bool kickNotes[numDrumSteps] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
bool kickStarted = 0; //if the kick has been triggered already in the current step


void startNote(int noteToStart) {
  MIDI.sendNoteOn(noteToStart, 127, 2);
}

void stopNote(int noteToStop) {
  MIDI.sendNoteOff(noteToStop, 127, 2);
}

void nextDrumStep() {  
  if (drumStepPointer >= numDrumSteps) {
    drumStepPointer = 0;
  }

  stopNote(kickNote);

  //only send kick if not already done
  if (kickNotes[drumStepPointer]) {
    startNote(kickNote);
  }
    
  drumStepPointer++;
}

// what happens when a clock signal is received
void handleClock() {
  nextDrumStep();
 //startNote(midiC + transpose + 0);
}

void setup() {
  //Serial.begin(57600);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);
  delay(10);

  //set function to be called when a clock signal is received
  MIDI.setHandleClock(handleClock);

}


void loop() {
  
    readDrumpad(seq);
    MIDI.read();
}
