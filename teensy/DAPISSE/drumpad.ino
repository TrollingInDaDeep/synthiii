#include <Keypad.h>

// sets all steps for selected instrument to false
// means, clear all enabled triggers
void clearInstrumenSequence() {
  for (int i = 0; i < numDrumSteps; i++){
    drumSequence [selectedInstrument][i] = false;
  }
}


// trigger next step for drum sequencer
// stops the previous notes and triggers the new ones
void nextDrumStep() {  
  if (drumStepPointer >= numDrumSteps) {
    drumStepPointer = 0;
  }

  for (int i = 0; i < drumInstruments; i++) {
    stopDrumNote(instrumentNotes[i]);
  }
  
  for (int i = 0; i < drumInstruments; i++) {
    if (drumSequence[i][drumStepPointer]) {
      startDrumNote(instrumentNotes[i]);
    }
  }
  drumStepPointer++;
}

void startDrumNote(int noteToStart) {
  usbMIDI.sendNoteOn(noteToStart, 127, 2);
  //Serial.println(noteToStart);
  stopDrumNote(noteToStart);
}

void stopDrumNote(int noteToStop) {
  usbMIDI.sendNoteOff(noteToStop, 127, 2);
  //Serial.println(noteToStop);
}

/// sets the modifier of the drum tempo based on the sequencer clock
/// expects selector number between 0 and 13
void updateDrumTempoModifier(int modifyNumber) {
  switch (modifyNumber){
    case 0: // /16
      tempoModifier = 0.0625;
    break;

    case 1: // /8
      tempoModifier = 0.125;
    break;

    case 2: // /4
      tempoModifier = 0.125;
    break;

    case 3: // /3
      tempoModifier = 0.3333333;
    break;

    case 4: // /2
      tempoModifier = 0.5;
    break;

    case 5: // /1.5
      tempoModifier = 0.6666666;
    break;

    case 6: // 1
      tempoModifier = 1;
    break;

    case 7: // *1.5
      tempoModifier = 1.5;
    break;

    case 8: // *2
      tempoModifier = 2;
    break;

    case 9: // *3
      tempoModifier = 3;
    break;

    case 10: // *4
      tempoModifier = 4;
    break;

    case 11: // *8
      tempoModifier = 8;
    break;

    case 12: // *16
      tempoModifier = 16;
    break;
  }
}

//scans through the keypad
//checks if key is pressed -> runs needed action
//checks if key is released -> runs needed action
void readDrumpad() {
    // supports up to ten simultaneous key presses
      if (kpd.getKeys())  
    {
      // scan the whole key list
      for (int i = 0; i < howManyKeypadKeys; i++) 
      {
        // Seht selbst
        int keyNumber = youFuckenKiddingMe(kpd.key[i].kchar);

        // find keys that have changed state
        if ( kpd.key[i].stateChanged)   
        {
          // when keys are pressed
          if (kpd.key[i].kstate == PRESSED) 
          {
            switch (keypadMode) {
              //Play Mode
              case 0:
                startDrumNote(midiC + transpose + keyNumber);
              break;

              //enable / disable notes sequence Mode
              case 1:
                drumSequence[selectedInstrument][keyNumber] = !drumSequence[selectedInstrument][keyNumber];
              break;

              // Fill every x step Mode
              case 2:

              break;

              //Settings mode
              case 3:

              break;
            }
          }


          // when keys are released
          if (kpd.key[i].kstate == RELEASED)  
          {

            switch (keypadMode) {
              //Play Mode
              case 0:
                stopDrumNote(midiC + transpose + keyNumber);
              break;

              //enable / disable notes sequence Mode
              case 1:

              break;

              // Fill every x step Mode
              case 2:

              break;

              //Settings mode
              case 3:

              break;
            }
            
          }
            
        }
      }
    }
}

    
