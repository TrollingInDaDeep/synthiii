/// reads the drumpad buttons
void readDrumPad() {

  bool buttonChanged = false;
  //loop through all buttons
  for (int r = 0; r < 4; r++){
    for (int c = 0; c < 4; c++){
      if (telephone[0].ButtonStates[c][r] != keypadMatrix.getPrevState(c, r)){ //if state not the same
        telephone[0].lastButtonStates[c][r] = telephone[0].ButtonStates[c][r]; //store old value in "last"
        telephone[0].ButtonStates[c][r] = keypadMatrix.getPrevState(c, r); //store new value in "current"
        buttonChanged = true;
      }
    }

  }

  //only run operands once at change, not multiple times
  
    
    if (telephone[0].keypadMode == 3) { //setRate mode
      if (buttonChanged){
        int col;
        col = 0;

        //Loop through col 1 -> decrease rate
        for (int i = 0; i < 4; i++){
          if (!telephone[0].ButtonStates[col][i]) {
            //Serial.print("decrease ");
            //Serial.println(i);
            changeDrumTriggerFreq(i+1,false); //drum instruments are id 1-4 currently
          }
        }

        col = 1; //mute
        for (int i = 0; i < 4; i++){
          if (!telephone[0].ButtonStates[col][i]) {
            subClocks[i+1].run = !subClocks[i+1].run;
            
          }
        }

        col = 2; //increase
        //Loop through col 3 -> increase rate
        for (int i = 0; i < 4; i++){
          if (!telephone[0].ButtonStates[col][i]) {
            //Serial.print("increase ");
            //Serial.println(i);
            changeDrumTriggerFreq(i+1,true); //drum instruments are id 1-4 currently
          }
        }

        col = 3; //play note
        //Loop through col 4 -> play note
        for (int i = 0; i < 4; i++){
          if (!telephone[0].ButtonStates[col][i]) {
            stopDrumNote(i+1); //weak de-doubling algorithm
            startDrumNote(i+1); //subclockID starts at 1 for drum instruments
          }
        }
      }
    }



    if (telephone[0].keypadMode == 4) { //I need a drummer
      if (buttonChanged){
        
        int btnCounter = 0; //counts which button we're at
        for (int col = 0; col < 4; col++){
          for (int row = 0; row < 4; row++){
            if (!telephone[0].ButtonStates[col][row]) {
              //Serial.print("Button pressed: ");
              //Serial.println(btnCounter);

              switch(btnCounter){
                case 0: telephone[0].drummerGenre = 0; break;
                case 4: telephone[0].drummerGenre = 1; break;
                case 8: telephone[0].drummerGenre = 2; break;
                case 1: telephone[0].drummerGenre = 3; break;
                case 5: telephone[0].drummerGenre = 4; break;
                case 9: telephone[0].drummerGenre = 5; break;
                case 2: telephone[0].drummerGenre = 6; break;
                case 6: telephone[0].drummerGenre = 7; break;
                case 10: telephone[0].drummerGenre = 8; break;
                case 7: telephone[0].drummerGenre = 9; break;

                case 3: //decrease intensity
                  changeDrummerIntensity(false);
                break;

                case 11: //increase intensity
                  changeDrummerIntensity(true);
              }
              Serial.print("btn: ");
              Serial.print(btnCounter);
              Serial.print(" | Genre: ");
              Serial.print(telephone[0].drummerGenre);
              Serial.print(" | Int: ");
              Serial.println(telephone[0].drummerIntensity);
            }
            btnCounter++;
          }
        }
        
        ///lookup shite below
        // int col;
        // col = 0;

        // //Loop through col 1 -> decrease intensity
        // for (int i = 0; i < 4; i++){
        //   if (!telephone[0].ButtonStates[col][i]) {
        //     Serial.print("decrease ");
        //     Serial.println(i);
        //     changeDrumTriggerFreq(i+1,false); //drum instruments are id 1-4 currently
        //   }
        // }

        // col = 1; //mute
        // for (int i = 0; i < 4; i++){
        //   if (!telephone[0].ButtonStates[col][i]) {
        //     subClocks[i+1].run = !subClocks[i+1].run;
            
        //   }
        // }

        // col = 2; //increase
        // //Loop through col 3 -> increase rate
        // for (int i = 0; i < 4; i++){
        //   if (!telephone[0].ButtonStates[col][i]) {
        //     Serial.print("increase ");
        //     Serial.println(i);
        //     changeDrumTriggerFreq(i+1,true); //drum instruments are id 1-4 currently
        //   }
        // }

        // col = 3; //play note
        // //Loop through col 4 -> play note
        // for (int i = 0; i < 4; i++){
        //   if (!telephone[0].ButtonStates[col][i]) {
        //     stopDrumNote(i+1); //weak de-doubling algorithm
        //     startDrumNote(i+1); //subclockID starts at 1 for drum instruments
        //   }
        //}
      }

    }
  
}

///changes the triggerfrequency of a drum subclock
/// subclock: subclockID
/// direction: true = up, false = down
void changeDrumTriggerFreq(int subClock, bool direction){
  if (!direction) { //decrease
    subClocks[subClock].triggerFrequency--; //decrease frequency

    //if 0 or lower, keep at 0
    if (subClocks[subClock].triggerFrequency <= 0) {
      subClocks[subClock].triggerFrequency = 0;
    }
  } else { //increase frequency
    subClocks[subClock].triggerFrequency++; //increase frequency
    //if max. or higher, keep at max.
    if (subClocks[subClock].triggerFrequency >= numTriggerFrequencies) {
      subClocks[subClock].triggerFrequency = numTriggerFrequencies-1;
    }
  }

  Serial.println(subClocks[subClock].triggerFrequency);
}

/// changes intensity of drummer
/// direction: true = up, false = down
void changeDrummerIntensity(bool direction){
  if (!direction) { //decrease
    telephone[0].drummerIntensity--;

    //if 0 or lower, keep at 0
    if (telephone[0].drummerIntensity <= 0) {
      telephone[0].drummerIntensity = 0;
    }
  } else { //increase
    telephone[0].drummerIntensity++; //increase intensity
    //if max. or higher, keep at max.
    if (telephone[0].drummerIntensity >= numDrummerIntensity) {
      telephone[0].drummerIntensity = numDrummerIntensity-1;
    }
  }
}

///starts a drumnote reading note from drumInstrument array
void startDrumNote(int subClockID) {

  usbMIDI.sendNoteOn(drumInstrumentNotes[subClocks[subClockID].instrument], 127, subClocks[subClockID].midiChannel);
}

///stops a drumnote reading note from drumInstrument array
void stopDrumNote(int subClockID) {
  usbMIDI.sendNoteOff(drumInstrumentNotes[subClocks[subClockID].instrument], 127, subClocks[subClockID].midiChannel);
}