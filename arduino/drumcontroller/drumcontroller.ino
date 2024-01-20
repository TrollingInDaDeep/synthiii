// Midi Controller Prototype
// 1 Multiplexer on A0 reading 8 POTs
// Multiplexer CD4051 control Pins on 10,11,12 (A, B, C)
// Keypad on pins 2-9                                                                              
// 24.10.2021 by dri and se internet under bedingungen
// where he just refused to be a larry and just play games
// however he will battlefield 4 right afterwards
// ... if this shit here is working

#include <Keypad.h>
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// drum definitions
// number of instruments (0=kick, 1=snare, 2=highhat, 3=cymbal)
const int drumInstruments = 4;
// which instrument is currently selected for keypad play mode
int selectedInstrument = 0;

const int instrumentNotes[] = {60, 75, 63, 67};

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

void startNote(int noteToStart) {
  MIDI.sendNoteOn(noteToStart, 127, 2);
  stopNote(noteToStart);
}

void stopNote(int noteToStop) {
  MIDI.sendNoteOff(noteToStop, 127, 2);
}

void nextDrumStep() {  
  if (drumStepPointer >= numDrumSteps) {
    drumStepPointer = 0;
  }

  for (int i = 0; i < drumInstruments; i++) {
    stopNote(instrumentNotes[i]);
  }
  
  for (int i = 0; i < drumInstruments; i++) {
    if (drumSequence[i][drumStepPointer]) {
      startNote(instrumentNotes[i]);
    }
  }
  drumStepPointer++;
}

// what happens when a clock signal is received
void handleClock() {
  if (runDrum) {
    nextDrumStep();
  }
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
  
    readDrumpad();
    MIDI.read();
}
