// Midi Controller Prototype
// 1 Multiplexer on A0 reading 8 POTs
// Multiplexer CD4051 control Pins on 10,11,12 (A, B, C)
// Keypad on pins 2-9                                                                              
// 24.10.2021 by dri and se internet under bedingungen
// where he just refused to be a larry and just play games
// however he will battlefield 4 right afterwards
// ... if this shit here is working

#include <Keypad.h>

///
/// Variable definitions
///

// ############
// DRUMS
// ############
int midiC = 60; // Standard midi C
int transpose = 0;

// number of instruments (0=kick, 1=snare, 2=highhat, 3=cymbal)
const int drumInstruments = 4;
// which midi note to play for each instrument
const int instrumentNotes[] = {60, 75, 63, 67};
// which instrument is currently selected for keypad play mode
int selectedInstrument = 0;



//number of notes / steps the drum sequencer has
const int numDrumSteps = 16;
// pointer at which step in the drum sequencer we are
int drumStepPointer = 0;

int keypadMode = 1; //0=play, 1=sequence notes 2=fillXStep, 4=settings
bool runDrum = true;

//bools if a drum sound should be triggered at the selected step
bool drumSequence [drumInstruments][numDrumSteps] {
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
};

int howManyKeypadKeys = 16;
char drumPadChars [16] = {'1', '2', '3', '+', '4', '5', '6', '>', '7', '8', '9', '!', '*', '0', '#', 'R'};


// ############
// KEYPAD
// ############
const byte ROWS = 4;
const byte COLS = 4;
//hex because only 1 digit char possible
char hexaKeys[ROWS][COLS] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', 'A', 'B'},
  {'C', 'D', 'E', 'F'}
};

// flachbandkabel:
// weisser leerer pin links
// R1, R2, C1, C2, C3, C4, R3, R4
byte rowPins[ROWS] = {2, 3, 9, 8}; 
byte colPins[COLS] = {4, 5, 6, 7}; 
Keypad kpd = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


///
/// End variable definitions
///


///
/// Functions
///


// what happens when a clock signal is received
void handleClock() {
  if (runDrum) {
    nextDrumStep();
  }
}

void setup() {
  // Pin definitions



  
  //usbMIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);
  delay(10);

  //set function to be called when a clock signal is received
  usbMIDI.setHandleClock(handleClock);

}


void loop() {
    readDrumpad();
    usbMIDI.read();

}
