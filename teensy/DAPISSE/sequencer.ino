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

  if (arr_seq_buttons[1][stepPointer]) {
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
  //digitalWrite(muxLEDIO, LOW);
}

void readAnalogPins() {
  // loop through analog in multiplexers
  for (int mux = 0; mux<numAnalogInMux; mux++){
    
    //loop through every pin for the analog muxes
    for (byte pin=0; pin<=7; pin++){
        selectMuxInPin(pin);
        // only read if input is not disabled
        if (!arr_disable_analog_inputs[mux][pin]){
          rawAnalog = analogRead(arr_pin_analog_inputs[mux]);
          arr_prev_read_analog_inputs[mux][pin] = arr_read_analog_inputs[mux][pin];
          arr_read_analog_inputs[mux][pin] = map(rawAnalog,0,1023,0,127);
        }
        
        if (arr_prev_read_analog_inputs[mux][pin] != arr_read_analog_inputs[mux][pin]) {
          // Serial.print("mux ");
          // Serial.print(mux);
          // Serial.print(" pin ");
          // Serial.print(pin);
          // Serial.print(" reads: ");
          // Serial.println(arr_read_analog_inputs[mux][pin]);
          usbMIDI.sendControlChange(arr_send_analog_inputs[mux][pin], arr_read_analog_inputs[mux][pin], 1);
        }
      }
  }
}