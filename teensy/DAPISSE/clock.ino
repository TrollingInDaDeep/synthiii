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
    timeSinceLastTick = currentMillis - lastTickTime;
    missedTicks = timeSinceLastTick / tickMS; // how many ticks missed since last read
    
    if (missedTicks > 0) {
      for (int j = 0; j < missedTicks; j++){
        //old working code//currentTick = (elapsedTime / tickMS); //division with int will only yield whole numbers.
        currentTick = (currentTick + 1) % subTicks; //move to next tick, reset to 0 when subTicks reached
      
        if (currentTick == 0){
          nextClockCycle();
        }
        nextTick();
     
      }
      
      //update based on how many ticks have been processed
      lastTickTime = currentMillis - fmod(timeSinceLastTick, tickMS);
    }
 
    elapsedTime = currentMillis - clockStart;

    // suggested by ChatGPT to update this more frequently
    tickMS = tempo / subTicks;

    
    //Serial.println(currentTick);
    updateClockTempo();

    for (int i = 0; i < numSubClocks; i++){
      if (currentMillis - subClocks[i][11] >= subClocks[i][7] && subClocks[i][10] == 0) { //if gate time is over and stop not yet sent
          subClocks[i][9] = 0; //set note to STOP mode
          clockHandler(i); // should stop the Note
          subClocks[i][10] = 1;
      }
    }
  }
}

//should get the current tick, maybe shitty
int getCurrentTick() {
  int counter = 1;
  for (int leftTicks = 1; leftTicks <= subTicks; leftTicks++) {
    if (((currentMillis - clockStart)-(leftTicks * tickMS)) > 0){
      counter++;
    }
  }
  return counter;
}

//Run actions needed for next Tick
void nextTick() {
  lastTick = currentTick;
  for (int i = 0; i < numSubClocks; i++){
    subClocks[i][12]++; //increase the tick counter
    if (subClocks[i][12] >= subClocks[i][5]) { // No ticks left -> trigger!
      clockHandler(i);

      //reset ticksLeft: Tick + delay
      subClocks[i][5] = subClocks[i][3] + subClocks[i][4]; //necessary?

      //reset tick counter for subclock. 
      subClocks[i][12] = 0;
    }
  }
}

//run actions needed for next Clock Cycle (beat)
void nextClockCycle() {
  prevClockStart = clockStart;
  clockStart = currentMillis;
  currentTick = 0;
  lastTick = 0; //that's the solution++ otherwise it will drift!!!!!

  for (int i = 0; i < numSubClocks; i++)
  {
    if (subClocks[i][12] < subClocks[i][5]) { // if still ticks left, don't use delay
      subClocks[i][4] = 0;
    }
    else
    {
      subClocks[i][4] = subClocks[i][13]; //load delay from buffer if no ticks left 
    }
  }
  //Serial.print("clock @");
  //Serial.println(currentTick);
  usbMIDI.sendClock();
  // Serial.println(millis());
}



//update tempo
/// sets the tempo modifier of the subclock
/// expects number to multiply or divide clock
void updateClockTempo() {

  //calculate the tempo division and multiplication
  for (int i = 0; i < numSubClocks; i++)
    {
      // division of tempo
      if (subClocks[i][2] == 1){
        subClocks[i][3] = subTicks / subClocks[i][1]; // ticks = subTicks / ratio
      }
      //Multiplication of tempo
      else
      {
        subClocks[i][3] = subTicks * subClocks[i][1]; // ticks = subTicks * ratio
      }
    }

  if (oldBPM != bpm){ //only update if bpm changed
    oldBPM = bpm;
    tempo = 1000.0/(bpm/60.0);
    tickMS = tempo/subTicks;

    for (int i = 0; i < numSubClocks; i++)
    {
      subClocks[i][5] = subClocks[i][3] + subClocks[i][4]; // Reset ticks left for each subclock
    }

  
    // Serial.print("BPM: ");
    // Serial.print(bpm);
    // Serial.print("¦ tickMS: ");
    // Serial.print(tickMS);
    // Serial.print("¦ ticksLeft: ");
    // Serial.println(subClocks[0][5]);
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
          
          //Serial.print("pulse Sequencer @");
          //Serial.println(currentTick);
          
          subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
          subClocks[subClockID][11] = currentMillis;
        }
        else { // note Stop
          //Serial.println("noteStopMode");
          if (!noteStopped) {
            stopLastNote();
          }
          subClocks[subClockID][9] = 1; //set note to START mode
        }
      }
    break;

    case 1 ... 4:
      if (subClocks[subClockID][8] == 1){ // if subclock is running
        if (subClocks[subClockID][9] == 1){ //note Start
          startDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
          //Serial.print("pulse Kick @");
          //Serial.println(currentTick);
          
          subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
          subClocks[subClockID][11] = currentMillis;
        }
        else { // note Stop
          if (!drumNoteStopped[0]) {
            stopDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
          }
          subClocks[subClockID][9] = 1; //set note to START mode
        }
      }
    break;

    // case 2: // snare
    //   if (subClocks[subClockID][8] == 1){ // if subclock is running
    //     if (subClocks[subClockID][9] == 1){ //note Start
    //       startDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
    //       //Serial.print("pulse snare @");
    //       //Serial.println(currentTick);
          
    //       subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
    //       subClocks[subClockID][11] = currentMillis;
    //     }
    //     else { // note Stop
    //       if (!drumNoteStopped[0]) { //snare
    //         stopDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
    //       }
    //       subClocks[subClockID][9] = 1; //set note to START mode
    //     }
    //   }
    // break;

    // case 3: // hat
    //   if (subClocks[subClockID][8] == 1){ // if subclock is running
    //     if (subClocks[subClockID][9] == 1){ //note Start
    //       startDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
    //       //Serial.print("pulse snare @");
    //       //Serial.println(currentTick);
          
    //       subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
    //       subClocks[subClockID][11] = currentMillis;
    //     }
    //     else { // note Stop
    //       if (!drumNoteStopped[0]) { //snare
    //         stopDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
    //       }
    //       subClocks[subClockID][9] = 1; //set note to START mode
    //     }
    //   }
    // break;
  }
  
}

// Reset
void resetClock() {
  Serial.println("reset Clock");
  tickMS = tempo/subTicks;
  currentTick = 0;
  lastTick = 0;
  lastTickTime = millis();
  clockStart = 0;
  prevClockStart = 0;
  elapsedTime = 0;
  currentMillis = millis();

  updateClockTempo();
  //Ensure ticksLeft are correctly initialized for the new cycle
  for (int i = 0; i < numSubClocks; i++) {
    //load delay from delay buffer
    subClocks[i][4] = subClocks[i][13];
    subClocks[i][5] = subClocks[i][3] + subClocks[i][4];  // Reset to tick + delay
    subClocks[i][12] = 0; //reset the clock Counter
    subClocks[i][4] = 0; //clear the delay again as it shall only be used once
  }

  nextClockCycle();
}