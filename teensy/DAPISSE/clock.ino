// reading the code of clocked vcv rack module
// https://github.com/MarcBoule/ImpromptuModular/blob/master/src/Clocked.cpp#L652
// realizing a separate master clock is a way better idea
// clocked is a great module


//external clock handler
//tap tempo


///
/// Functions
///

// updateClock -> run several times also during digital Read
// check if any action to do (tick or subtick)
// update subticks
void updateClock() {
  if (run){
    currentMillis = millis();
    elapsedTime = currentMillis - clockStart;

    // suggested by ChatGPT to update this more frequently
    tickMS = tempo / numTicks;

    currentTick = (elapsedTime / tickMS); //division with int will only yield whole numbers.
    //Serial.println(currentTick);
    updateClockTempo();

    for (int i = 0; i < numSubClocks; i++){
      if (currentMillis - subClocks[i][11] >= subClocks[i][7] && subClocks[i][10] == 0) { //if gate time is over and stop not yet sent
          subClocks[i][9] = 0; //set note to STOP mode
          clockHandler(i); // should stop the Note
          subClocks[i][10] = 1;
      }
    }
    
    if (currentTick != lastTick) {
      nextTick();
    }
    
    if (elapsedTime >= tempo) {
      nextClockCycle();
    }    
  }
}

//should get the current tick, maybe shitty
int getCurrentTick() {
  int counter = 1;
  for (int leftTicks = 1; leftTicks <= numTicks; leftTicks++) {
    if (((currentMillis - clockStart)-(leftTicks * tickMS)) > 0){
      counter++;
    }
  }
  return counter;
}

//Run actions needed for next Tick
void nextTick() {
  lastTick = currentTick;
  Serial.println(subClocks[0][5]); // ticks left
  for (int i = 0; i < numSubClocks; i++){
    //if (subClocks[i][5] > 0){// don't decrease when already 0, don't kick downwards pls
    subClocks[i][5]--; //decrease the ticks left
    //}
    if (subClocks[i][5] < 1) { // No ticks left -> trigger!
      clockHandler(i);
      //reset tick counter for subclock. Tick + delay
      subClocks[i][5] = subClocks[i][3] + subClocks[i][4];
    }
  }
}

//run actions needed for next Clock Cycle (beat)
void nextClockCycle() {
  prevClockStart = clockStart;
  clockStart = currentMillis;
  currentTick = 0;
  lastTick = 0; //that's the solution++ otherwise it will drift!!!!!

  Serial.print("clock @");
  Serial.println(currentTick);
  usbMIDI.sendClock();
  // Serial.println(millis());
}



//update tempo
/// sets the tempo modifier of the subclock
/// expects number to multiply or divide clock
void updateClockTempo() {
  
  if (oldBPM != bpm){ //only update if bpm changed
    oldBPM = bpm;
    tempo = 1000.0/(bpm/60.0);
    tickMS = tempo/numTicks;

    for (int i = 0; i < numSubClocks; i++)
    {
      subClocks[i][5] = subClocks[i][3] + subClocks[i][4]; // Reset ticks left for each subclock
    }
  }

  //calculate the tempo division and multiplication
  for (int i = 0; i < numSubClocks; i++)
    {
      // division of tempo
      if (subClocks[i][2] == 1){
        subClocks[i][3] = numTicks / subClocks[i][1]; // ticks = numTicks / ratio
      }
      //Multiplication of tempo
      else
      {
        subClocks[i][3] = numTicks * subClocks[i][1]; // ticks = numTicks * ratio
      }
    }
  
}

 //trigger actions for each subclock
 //either seq step
 //or drum instrument trigger
 //or note stop -> maybe should be handled via gate time of each instrument by itself
void clockHandler (int subClockID) {
  switch (subClockID)
  {
    case 0: // sequencer
    if (subClocks[subClockID][8] == 1){ // if subclock is running
      if (subClocks[subClockID][9] == 1){ //note Start
        nextPulse();
        
        //usbMIDI.sendNoteOn(60, velocity, synthMidiChannel);
        Serial.print("pulse Sequencer @");
        Serial.println(currentTick);
        
        subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
        subClocks[subClockID][11] = currentMillis;
      }
      else { // note Stop
        Serial.println("noteStopMode");
        if (!noteStopped) {
          stopLastNote();
        }
        //usbMIDI.sendNoteOff(60, velocity, synthMidiChannel);
        subClocks[subClockID][9] = 1; //set note to START mode
      }
    }
  }
  
}

// Reset
void resetClock() {
  Serial.println("reset Clock");
  tickMS = tempo/numTicks;
  currentTick = 0;
  lastTick = 0;
  clockStart = 0;
  prevClockStart = 0;
  elapsedTime = 0;
  currentMillis = millis();

  updateClockTempo();
  //Ensure ticksLeft are correctly initialized for the new cycle
  for (int i = 0; i < numSubClocks; i++) {
    subClocks[i][5] = subClocks[i][3] + subClocks[i][4];  // Reset to tick + delay 
  }

  nextClockCycle();
}