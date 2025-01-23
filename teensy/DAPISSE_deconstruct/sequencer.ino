// all functions for the sequencer
// or most
void noteButtonPressed(int note) {
  startNote(note);
}

void noteButtonReleased(int note){
  stopNote(note);
}

///obsolete due to new clock
// // functions
// void updateTempo(){
//   tempo = 1000.0/(bpm/60.0);
//   tickMS = tempo/24;
//   updateDrumTempo();
// }


void nextStep() {
  
  //Sequencer play order
  switch (playMode) {
    //0=forwards
    case 0:
      stepPointer++;
      if (stepPointer >= numSteps || stepPointer < 0) {
        stepPointer = 0;
      }
      seqDirection = 1; //upwards
    break;

    //1=backwards
    case 1:
      stepPointer--;
      if (stepPointer < 0) {
        stepPointer = numSteps-1;
      }
      seqDirection = 0; //downwards
    break;

    //2=ping-pong
    case 2:
      if (seqDirection) {
        stepPointer++;
      } else {
        stepPointer--;
      }

      if (stepPointer < 0) {
        stepPointer = 1;
        seqDirection = 1; //go upwards
      }
      if (stepPointer >= numSteps){
        stepPointer = numSteps-2;
        seqDirection = 0; // go downwards
      }
    break;

    //3=drunk
    case 3:
      stepPointer = random(0,numSteps);
    break;
  }
  // check if step should be skipped
  if (arr_seq_buttons[1][stepPointer]) {
    nextStep();
  }
  selectMuxOutPin(byte(stepPointer));
}


//buggy, not sure if needed
void stopLastNote(){
  usbMIDI.sendNoteOff(lastNoteSent, velocity, synthMidiChannel);
  subClocks[0][10] = 1; //select sequencer, set "stopSent" to true

}

void startNote(int noteToPlay){
  stopLastNote();

  //set slide back to 0
  usbMIDI.sendControlChange(20, 0, synthMidiChannel); //20 is the slide ctrlr number

  //if slide for note enabled -> enable slide control
  if (arr_seq_buttons[2][noteToPlay]){
      usbMIDI.sendControlChange(20, slideAmount, synthMidiChannel); //20 is the slide ctrlr number
  }
  //noteStart = millis();
  usbMIDI.sendNoteOn(arr_seq_buttons[0][noteToPlay], velocity, synthMidiChannel);
  subClocks[0][10] = 0; //select sequencer, set "stopSent" to false
  lastNoteSent=arr_seq_buttons[0][noteToPlay];
  digitalWrite(I7, HIGH);
}

void nextPulse() {
  //go to next pulse
  //pulseStart = millis();

  //go first pulse, first step if last pulse is reached or reset is triggered (pulsePointer = -1)
  if (pulsePointer >= pulseCount[stepPointer] || pulsePointer < 0) {
    pulsePointer = 0;
    nextStep();
  }
  if (run) {
    startNote(stepPointer); // start the note of this pulse
  }
  pulsePointer++;
}

void stopNote(int noteToStop){
  usbMIDI.sendNoteOff(arr_seq_buttons[0][noteToStop], velocity, synthMidiChannel);
  subClocks[0][10] = 1; //select sequencer, set "stopSent" to true
  //digitalWrite(I7, LOW);
}

// todo convert fusel0 and fusel1 to int number and store in seqButtonFunction


// reset immediately and trigger the first pulse of the first step
void resetSequencer() {
  stepPointer = -1;
  pulsePointer = -1;
  nextPulse(); //experimental, might need to uncomment again
  reset = false;

  ///OBSOLETE
  //prevClockStart = millis();
  //prevPulseStart = millis();
  // if (drumDivMultMode){
  //   for (int i = 0; i < numDrumInstruments; i++)
  //   {
  //     prevDrumNoteStart[i] = millis();
  //   }
  // }
  // if (syncDrumToSequencer){
  //     drumStepPointer = -1;
  // }
  // if (syncDrumToSequencer){
  //   nextDrumStep();
  // }
  
}

