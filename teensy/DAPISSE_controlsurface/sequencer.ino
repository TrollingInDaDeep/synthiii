// all functions for the sequencer
// or most
void noteButtonPressed(int note) {
  startNote(note);
}

void noteButtonReleased(int note){
  stopNote(note);
}



void nextStep() {
  //Sequencer play order
  switch (Metropolis[0].playMode) {
    //0=forwards
    case 0:
      Metropolis[0].stepPointer++;
      if (Metropolis[0].stepPointer >= Metropolis[0].numSteps || Metropolis[0].stepPointer < 0) {
        Metropolis[0].stepPointer = 0;
      }
      Metropolis[0].seqDirection = 1; //upwards
    break;

    //1=backwards
    case 1:
      Metropolis[0].stepPointer--;
      if (Metropolis[0].stepPointer < 0) {
        Metropolis[0].stepPointer = Metropolis[0].numSteps-1;
      }
      Metropolis[0].seqDirection = 0; //downwards
    break;

    //2=ping-pong
    case 2:
      if (Metropolis[0].seqDirection) {
        Metropolis[0].stepPointer++;
      } else {
        Metropolis[0].stepPointer--;
      }

      if (Metropolis[0].stepPointer < 0) {
        Metropolis[0].stepPointer = 1;
        Metropolis[0].seqDirection = 1; //go upwards
      }
      if (Metropolis[0].stepPointer >= Metropolis[0].numSteps){
        Metropolis[0].stepPointer = Metropolis[0].numSteps-2;
        Metropolis[0].seqDirection = 0; // go downwards
      }
    break;

    //3=drunk
    case 3:
      Metropolis[0].stepPointer = random(0,Metropolis[0].numSteps);
    break;
  }
  // check if step should be skipped
  if (seqSteps[Metropolis[0].stepPointer].skip) {
    nextStep();
  }
  //   Serial.print("Step ");
  //   Serial.print(Metropolis[0].stepPointer);
  //   Serial.print("| direction ");
  //   Serial.print(Metropolis[0].seqDirection);
  // Serial.print("| sent to selector:");
  // Serial.println(Metropolis[0].stepPointer);
  selectMuxOutPin(Metropolis[0].stepPointer);
}

//buggy, not sure if needed
void stopLastNote(){
  usbMIDI.sendNoteOff(Metropolis[0].lastNoteSent, Metropolis[0].velocity, Metropolis[0].synthMidiChannel);
  subClocks[0].stopSent = 1; //select sequencer, set "stopSent" to true
}

void startNote(int noteToPlay){
  stopLastNote();

  //set slide back to 0
  usbMIDI.sendControlChange(22, 0, Metropolis[0].synthMidiChannel); //22 is the slide ctrlr number

  //if slide for note enabled -> enable slide control
  if (seqSteps[noteToPlay].slide){
      usbMIDI.sendControlChange(22, Metropolis[0].slideAmount, Metropolis[0].synthMidiChannel); //22 is the slide ctrlr number
  }
  //noteStart = millis();

  usbMIDI.sendNoteOn(seqSteps[noteToPlay].note, Metropolis[0].velocity, Metropolis[0].synthMidiChannel);
  subClocks[0].stopSent = false; //select sequencer, set "stopSent" to false
  Metropolis[0].lastNoteSent=seqSteps[noteToPlay].note;
  digitalWrite(I7, HIGH);
}

void nextPulse() {
  //go to next pulse
  //pulseStart = millis();
  //go first pulse, first step if last pulse is reached or reset is triggered (pulsePointer = -1)
  if (Metropolis[0].pulsePointer >= seqSteps[Metropolis[0].stepPointer].pulseCount || Metropolis[0].pulsePointer < 0) {
    Metropolis[0].pulsePointer = 0;
    nextStep();
  }
  if (Metropolis[0].run) {
    switch (seqSteps[Metropolis[0].stepPointer].gateMode){
      case 0: //no gate
        //nothing
      break;
      case 1: //first gate
        Serial.println(Metropolis[0].pulsePointer);
        if(Metropolis[0].pulsePointer == 0) {
          
          startNote(Metropolis[0].stepPointer); // start the note on first pulse
        }
      break;
      case 2: //every gate
        startNote(Metropolis[0].stepPointer); // start the note of this pulse
      break;

    }
    
  }
  Metropolis[0].pulsePointer++;
}

void stopNote(int noteToStop){
  usbMIDI.sendNoteOff(seqSteps[noteToStop].note, Metropolis[0].velocity, Metropolis[0].synthMidiChannel);
  subClocks[0].stopSent = true; //select sequencer, set "stopSent" to true
  //digitalWrite(I7, LOW);
}


// reset immediately and trigger the first pulse of the first step
void resetSequencer() {
  Metropolis[0].stepPointer = -1;
  Metropolis[0].pulsePointer = -1;
  nextPulse(); //experimental, might need to uncomment again
  Metropolis[0].reset = false;
}

//sets the function of the Sequencer trigger buttons
void selectSeqNoteFunction(){
  if (Metropolis[0].fuSel0 && Metropolis[0].fuSel1){
    //Play Mode
    Metropolis[0].buttonFunction = 0;
  }
  if (!Metropolis[0].fuSel0 && Metropolis[0].fuSel1){
    //Skip mode
    Metropolis[0].buttonFunction = 1;
  }
  if (Metropolis[0].fuSel0 && !Metropolis[0].fuSel1){
    //Slide mode
    Metropolis[0].buttonFunction = 2;
  }
  if (!Metropolis[0].fuSel0 && !Metropolis[0].fuSel1){
    //Hold mode --> idea: play between pressed buttons mode
    Metropolis[0].buttonFunction = 3;
  }
}
