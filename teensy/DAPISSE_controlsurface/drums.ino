/// reads the Drumpad keys and does actions depending on keypad mode
void readDrumPad(){
  switch (telephone[0].keypadMode){
    case 1: //Play Mode
      //do nothing
    break;

    case 2: //setRate
      //TODO: Read all keys, do the function they serve
      
      if (telephone[0].keypadMode == 1) {
          int row;
          //Down
          row = 0;
          for (int col = 0; col < 4; col++){
              switch (col){
                case 0: //Kick
                  if (keypadMatrix.getPrevState(row, col)){
                    //set kick rate +1, wrap at 14, maybe use a function
                  }
                break;
              }
              
          }

          //UP

          //Mute
          
            for (int col = 0; col < 4; col++){
              
            }
          }

    break;
  }

  
  }
  Serial.print(keypadMatrix.getPrevState(0, 0));
  Serial.print(keypadMatrix.getPrevState(0, 1));
  Serial.print(keypadMatrix.getPrevState(0, 2));
  Serial.print(keypadMatrix.getPrevState(0, 3));
  Serial.print(keypadMatrix.getPrevState(1, 0));
  Serial.print(keypadMatrix.getPrevState(1, 1));
  Serial.print(keypadMatrix.getPrevState(1, 2));
  Serial.print(keypadMatrix.getPrevState(1, 3));
  Serial.print(keypadMatrix.getPrevState(2, 0));
  Serial.print(keypadMatrix.getPrevState(2, 1));
  Serial.print(keypadMatrix.getPrevState(2, 2));
  Serial.print(keypadMatrix.getPrevState(2, 3));
  Serial.print(keypadMatrix.getPrevState(3, 0));
  Serial.print(keypadMatrix.getPrevState(3, 1));
  Serial.print(keypadMatrix.getPrevState(3, 2));
  Serial.println(keypadMatrix.getPrevState(3, 3));

}

///starts a drumnote reading note from drumInstrument array
void startDrumNote(int subClockID) {

  usbMIDI.sendNoteOn(drumInstrumentNotes[subClocks[subClockID].instrument], 127, subClocks[subClockID].midiChannel);
}

///stops a drumnote reading note from drumInstrument array
void stopDrumNote(int subClockID) {
  usbMIDI.sendNoteOff(drumInstrumentNotes[subClocks[subClockID].instrument], 127, subClocks[subClockID].midiChannel);
}