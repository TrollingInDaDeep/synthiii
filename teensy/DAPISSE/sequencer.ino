// all functions for the sequencer
// or most
void noteButtonPressed(int note) {
  startNote(note);
}

void noteButtonReleased(int note){
  stopNote(note);
}

// functions
void updateTempo(){
  tempo = 1000.0/(bpm/60.0);
  clockInterval = tempo/24;
}


void nextStep() {

  stepPointer++;

  //go to next step if all pulses are done
  if (stepPointer >= numSteps) {
    stepPointer = 0;
  }
  selectMuxOutPin(byte(stepPointer));
}


//buggy, not sure if needed
void stopLastNote(){
  usbMIDI.sendNoteOff(lastNoteSent, velocity, 1);
  noteStopped = true;

}

void startNote(int noteToPlay){
  stopLastNote();
  noteStart = millis();
  usbMIDI.sendNoteOn(arr_seq_buttons[0][noteToPlay], velocity, 1);
  noteStopped = false;
  lastNoteSent=arr_seq_buttons[0][noteToPlay];
  digitalWrite(I7, HIGH);
}

void nextPulse() {
  //go to next pulse
  pulseStart = millis();

  // check if step should be skipped
  if (arr_seq_buttons[1][stepPointer] == 1) {
    stepPointer++;
  }
  startNote(stepPointer); // start the note of this pulse

  pulsePointer++;

  //go to next step if last pulse is reached
  if (pulsePointer >= pulseCount[stepPointer]) {
    pulsePointer = 0;
    nextStep();
  }
}

void stopNote(int noteToStop){
  usbMIDI.sendNoteOff(arr_seq_buttons[0][noteToStop], velocity, 1);
  noteStopped = true;
  digitalWrite(I7, LOW);
}

// todo convert fusel0 and fusel1 to int number and store in seqButtonFunction
void selectSeqNoteFunction(){
seqButtonFunction
}

