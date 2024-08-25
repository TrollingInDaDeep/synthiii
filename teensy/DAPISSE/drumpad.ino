#include <Keypad.h>

// sets all steps for given instrument to false
// means, clear all enabled triggers
void clearInstrumentSequence(int instrument) {
  if(selectedDrumPattern == 1){
    for (int i = 0; i < numDrumSteps; i++){
      drumSequence[selectedDrumPattern][instrument][i] = false;
    }
  }
  
}


// trigger next step for drum sequencer
// stops the previous notes and triggers the new ones
void nextDrumStep() {  
  if (drumStepPointer >= numDrumSteps || drumStepPointer < 0) {
    drumStepPointer = 0;
  }

  stopAllDrumNotes();
  
  for (int i = 0; i < numDrumInstruments; i++) {
    if (drumSequence[selectedDrumPattern][i][drumStepPointer]) {
      if (runDrum){
      startDrumNote(instrumentNotes[1][i]);
      }
    }
  }
  drumStepPointer++;
}

void stopAllDrumNotes(){
  for (int i = 0; i < numDrumInstruments; i++) {
    stopDrumNote(instrumentNotes[1][i]);
  }
}

void startDrumNote(int noteToStart) {
  drumNoteStart = millis();
  usbMIDI.sendNoteOn(noteToStart, 127, drumMidiChannel);
  //Serial.println(noteToStart);
  //stopDrumNote(noteToStart);
  drumNoteStopped = false;
}

void stopDrumNote(int noteToStop) {
  usbMIDI.sendNoteOff(noteToStop, 127, drumMidiChannel);
  //Serial.println(noteToStop);
  drumNoteStopped = true;
}

/// select the next drum pattern
/// true = upwards, false = downwards
void changeDrumPattern(bool direction){
  if (direction){
    selectedDrumPattern++;
  }
  else {
    selectedDrumPattern--;
  }

  if (selectedDrumPattern >= numDrumPatterns && direction) {
    selectedDrumPattern = 0;
  }
  if (selectedDrumPattern < 0 && !direction) {
    selectedDrumPattern = numDrumPatterns-1;
  }
}
/// sets the modifier of the drum tempo based on the sequencer clock
/// expects number to multiply or divide clock
void updateDrumTempoModifier(int modifyNumber) {
  tempoModifier = modifyNumber;
    // Multiplication of tempo
    if (tempoOperation){
      instrumentNotes[2][selectedInstrument] = tempo * tempoModifier;
    }
    //Division of tempo
    else
    {
      instrumentNotes[2][selectedInstrument] = (double)tempo / tempoModifier;
    }



  Serial.println(tempoModifier);

  // old code, for selector, not the worst idea
  // switch (modifyNumber){
  //   case 0: // *16
  //     tempoModifier = 16;
  //   break;

  //   case 1: // *8
  //     tempoModifier = 8;
  //   break;

  //   case 2: // *4
  //     tempoModifier = 4;
  //   break;

  //   case 3: // *3
  //     tempoModifier = 3;
  //   break;

  //   case 4: // *2
  //     tempoModifier = 2;
  //   break;

  //   case 5: // *1.5
  //     tempoModifier = 1.5;
  //   break;

  //   case 6: // 1
  //     tempoModifier = 1;
  //   break;

  //   case 7: // /1.5
  //     tempoModifier = 0.6666666;
  //   break;

  //   case 8: // /2
  //     tempoModifier = 0.5;
  //   break;

  //   case 9: // /3
  //     tempoModifier = 0.3333333;
  //   break;

  //   case 10: // /4
  //     tempoModifier = 0.25;
  //   break;

  //   case 11: // /8
  //     tempoModifier = 0.125;
  //   break;

  //   case 12: // /16
  //     tempoModifier = 0.0625;
  //   break;
  // }
}

/// finds out which note to play for a key number
/// returns midi note. returns 0 if key is not a drumpad note
int getDrumNote(int keyNumber){
  int midiNote = 0;
  for (int i = 0; i < numDrumInstruments; i++)
    {
      if (keyNumber == instrumentNotes[0][i]){
        midiNote = instrumentNotes[1][i];
        selectedInstrument = i;
      }
    }
  return midiNote;
}

///records key into drum pattern
/// expects number of key on Keypad
void recordKey(int keyNumber){
    if (getDrumNote(keyNumber) != 0 ) {
      //what this should do:
      //when "playing" and "record" are enabled, enter played notes into the drumSequence
      //do that at the current step so it is quantized
      //works, but not always using the correct step to insert (musically)
      drumSequence[selectedDrumPattern][selectedInstrument][drumStepPointer] = true;
      Serial.print("INST ");
      Serial.print(keyNumber);
      Serial.print(" @Step ");
      Serial.println(drumStepPointer);
    }
}

//scans through the keypad
//checks if key is pressed -> runs needed action
//checks if key is released -> runs needed action
void readDrumpad() {
    int drumNote = 0;
    // supports up to ten simultaneous key presses
      if (kpd.getKeys())  
    {
      // scan the whole key list
      for (int i = 0; i < howManyKeypadKeys; i++) 
      {
        // Seht selbst
        int keyNumber = youFuckenKiddingMe(kpd.key[i].kchar);
        int keyValue = youFuckenKiddingMeToo(kpd.key[i].kchar);
        // find keys that have changed state
        if (kpd.key[i].stateChanged)   
        {
          // when keys are pressed
          if (kpd.key[i].kstate == PRESSED) 
          {
            drumNote = getDrumNote(keyNumber);
            switch (keypadMode) {
              //Play Mode
              case 0:
                if (recordDrum && runDrum && selectedDrumPattern == 1 && drumNote != 0){
                  recordKey(keyNumber);
                }
               
                if (drumNote != 0){
                  startDrumNote(drumNote + transpose);
                }

                switch (keyValue) {
                
                // fill each x step
                case 1 ... 9:
                  updateDrumTempoModifier(keyValue);
                break;


                case 13: // *
                  tempoOperation = true;
                break;

                case 14: // #
                  tempoOperation = false;
                break;
              }
                
              break;

              //enable / disable notes sequence Mode
              case 1:
                drumSequence[selectedDrumPattern][selectedInstrument][keyNumber] = !drumSequence[selectedDrumPattern][selectedInstrument][keyNumber];
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
            int drumNote = getDrumNote(keyNumber);
            switch (keypadMode) {
              //Play Mode
              case 0:
                switch (keyNumber) {
                  
                  case 13:
                    if (!holdFired){
                      changeDrumPattern(true);
                    }
                  break;
                }

                if (drumNote != 0){
                  stopDrumNote(drumNote + transpose);
                }
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
            holdFired = false;
          }

          if (kpd.key[i].kstate == HOLD)  
          {
            holdFired = true;
            switch (keyNumber) {
              case 13: // R key -> clear
                clearInstrumentSequence(0);
                clearInstrumentSequence(1);
                clearInstrumentSequence(2);
                clearInstrumentSequence(3);
              break;
            }
          }
            
        }
      }
    }
}

    
