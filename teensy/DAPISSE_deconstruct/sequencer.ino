

///obsolete due to new clock
// // functions
// void updateTempo(){
//   tempo = 1000.0/(bpm/60.0);
//   tickMS = tempo/24;
//   updateDrumTempo();
// }





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

