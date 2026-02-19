// reading the code of clocked vcv rack module
// https://github.com/MarcBoule/ImpromptuModular/blob/master/src/Clocked.cpp#L652
// realizing a separate master clock is a way better idea
// clocked is a great module

//TODO:
//external clock handler
//tap tempo


///
/// Functions
///

// updateClock -> run several times also during digital Read
// check if any action to do (tick or subtick)
// update subticks
void updateClock() {
  currentMicros = micros();
  //only if running and internal clock source selected
  if (mainClocks[0].run && mainClocks[0].clockSource == false){
    mainClocks[0].timeSinceLastTick = currentMicros - mainClocks[0].lastTickTime;
    mainClocks[0].missedTicks = mainClocks[0].timeSinceLastTick / mainClocks[0].tickMS; // how many ticks missed since last read
    
    if (mainClocks[0].missedTicks > 0) {
      for (int j = 0; j < mainClocks[0].missedTicks; j++){
        //old working code//currentTick = (elapsedTime / tickMS); //division with int will only yield whole numbers.
        mainClocks[0].currentTick = (mainClocks[0].currentTick + 1) % mainClocks[0].subTicks; //move to next tick, reset to 0 when subTicks reached
      
        if (mainClocks[0].currentTick == 0){
          nextClockCycle(false);
        }
        //nextTick();
        nextTick2();
      }
      
      //update based on how many ticks have been processed
      mainClocks[0].lastTickTime = currentMicros - fmod(mainClocks[0].timeSinceLastTick, mainClocks[0].tickMS);
    }
 
    mainClocks[0].elapsedTime = currentMicros - mainClocks[0].clockStart;

    // suggested by ChatGPT to update this more frequently
    mainClocks[0].tickMS = mainClocks[0].tempo / mainClocks[0].subTicks;

    
    //Serial.println(currentTick);
    updateClockTempo();

    for (int i = 0; i < numSubClocks; i++){
      if ((currentMicros - subClocks[i].startMS) / 1000  >= subClocks[i].gateTime && subClocks[i].stopSent == 0) { //if gate time is over and stop not yet sent
          subClocks[i].isStart = 0; //set note to STOP mode
          clockHandler(i); // should stop the Note
          subClocks[i].stopSent = 1;
      }
    }
  }
}



//should get the current tick, maybe shitty
int getCurrentTick() {
  int counter = 1;
  for (int leftTicks = 1; leftTicks <= mainClocks[0].subTicks; leftTicks++) {
    if (((currentMicros - mainClocks[0].clockStart)-(leftTicks * mainClocks[0].tickMS)) > 0){
      counter++;
    }
  }
  return counter;
}


///Run actions needed for next Tick -> currently unused
/// variant 1 based on subtracting ticks until 0 are left
void nextTick() {
  mainClocks[0].lastTick = mainClocks[0].currentTick;
  Serial.println(mainClocks[0].currentTick);
  for (int i = 0; i < numSubClocks; i++){
    subClocks[i].tickCounter++; //increase the tick counter
    if (subClocks[i].tickCounter >= subClocks[i].ticksLeft) { // No ticks left -> trigger!
      clockHandler(i);

      //reset ticksLeft: Tick + delay
      subClocks[i].ticksLeft = subClocks[i].tick + subClocks[i].delay; //necessary?

      //reset tick counter for subclock. 
      subClocks[i].tickCounter = 0;
    }
  }
    // if (runDrum){
  //   nextDrumStep(); #
  // }

}

/// Run actions needed for next Tick
/// variant 2 based on which tick we're currently in
void nextTick2() {
  //loop through subclocks
  for (int i = 0; i < numSubClocks; i++){
      
    //if current tick number is set to trigger in the triggerTable
    if (containsNum(subClocks[i].triggerFrequency, mainClocks[0].currentTick)){
      // todo: implement probability here #
      
      if (telephone[0].keypadMode != 4) {
        clockHandler(i);
      } else { // "i need a drummer" mode
        if (i < 1) { //sequencer
          clockHandler(i);
        }
      }
      
    }
    if (telephone[0].keypadMode == 4) {
      if (i >= 1 && i <= numDrumInstruments) { //drum instruments
        //check and loop through drummer brain
        if (drummerBrain[telephone[0].drummerGenre][telephone[0].drummerIntensity][telephone[0].drummerSeqBeat][i-1][mainClocks[0].currentTick]) { //if current beat, current subclock (drum instruments start at 0 again) and current tick are enabled ->trigger
          clockHandler(i);
        }
      }
    }
  }
}

/// checks if number (num) is contained in trigger table at index (idx)
/// returns bool if true
bool containsNum(int idx, int num) {
    for (int i = 0; i < rowSizes[idx]; i++) {
        if (triggerTable[idx][i] == num) {
            return true; // Found the number
        }
    }
    return false; // Not found
}




///run actions needed for next Clock Cycle (beat)
/// reset = true -> when called from reset
void nextClockCycle(bool reset) {
  mainClocks[0].prevClockStart = mainClocks[0].clockStart;
  mainClocks[0].clockStart = currentMicros;
  mainClocks[0].currentTick = 0;
  mainClocks[0].lastTick = 0; //that's the solution++ otherwise it will drift!!!!!

  if (!reset){
    increaseDrummerBeat();
  }
  else {
    telephone[0].drummerSeqBeat = 0;
  }
  // Serial.print("drm Beat ");
  // Serial.println(telephone[0].drummerSeqBeat);

  for (int i = 0; i < numSubClocks; i++)
  {
    if (subClocks[i].tickCounter < subClocks[i].ticksLeft) { // if still ticks left, don't use delay
      subClocks[i].delay = 0;
    }
    else
    {
      subClocks[i].delay = subClocks[i].delayBuffer; //load delay from buffer if no ticks left 
    }
  }
  //Serial.print("clock @");
  Serial.println("clock");
  //send midi clock via usb only if internal clock is used??
  if (mainClocks[0].clockSource == false){
    //dont send at every pulse, as korg needs only every 6th. handled via instrument
    //midi.sendClock();
    //midi.sendRealTime(RealTimeMessage::TimingClock);
  }
  

}

//update tempo
/// sets the tempo modifier of the subclock
/// expects number to multiply or divide clock
void updateClockTempo() {

  //calculate the tempo division and multiplication
  for (int i = 0; i < numSubClocks; i++)
    {
      // division of tempo
      if (subClocks[i].divMult == 1){
        subClocks[i].tick = mainClocks[0].subTicks / subClocks[i].ratio; // ticks = subTicks / ratio
      }
      //Multiplication of tempo
      else
      {
        subClocks[i].tick = mainClocks[0].subTicks * subClocks[i].ratio; // ticks = subTicks * ratio
      }
    }

  if (mainClocks[0].oldBPM != mainClocks[0].bpm){ //only update if bpm changed
    mainClocks[0].oldBPM = mainClocks[0].bpm;
    mainClocks[0].tempo = 1000000.0/(mainClocks[0].bpm/60.0);
    mainClocks[0].tickMS = mainClocks[0].tempo/mainClocks[0].subTicks;

    for (int i = 0; i < numSubClocks; i++)
    {
      subClocks[i].ticksLeft = subClocks[i].tick + subClocks[i].delay; // Reset ticks left for each subclock
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
void clockHandler(int subClockID) {
  switch (subClockID)
  {
    case 0: // sequencer
      if (subClocks[subClockID].run){ // if subclock is running
        if (subClocks[subClockID].isStart){ //note Start
          nextPulse();
          
          //Serial.print("pulse Sequencer @");
          //Serial.println(currentTick);
          
          subClocks[subClockID].stopSent = 0; //tell that stop hasn't been sent yet
          subClocks[subClockID].startMS = currentMicros;
        }
        else { // note Stop
          //Serial.println("noteStopMode");
          if (subClocks[subClockID].stopSent == 0) {
            stopLastNote();
          }
          subClocks[subClockID].isStart = 1; //set note to START mode
        }
      }
    break;

    case 1 ... 4: //all drum notes
      if (subClocks[subClockID].run){ // if subclock is running
        if (subClocks[subClockID].isStart){ //note Start
          
          float roll = random(0,101);
          if (roll <= subClocks[subClockID].triggerProb){
            // Serial.print(subClocks[subClockID].triggerProb);
            // Serial.print("bigger than roll: ");
            // Serial.println(roll);
            startDrumNote(subClockID);
          }
         
          
          subClocks[subClockID].stopSent = 0; //tell that stop hasn't been sent yet
          subClocks[subClockID].startMS = currentMicros;
        }
        else { // note Stop
          //Serial.println("noteStopMode");
          if (subClocks[subClockID].stopSent == 0) {
            stopDrumNote(subClockID);
          }
          subClocks[subClockID].isStart = 1; //set note to START mode
        }
      }
    break;

    case 5: //Korg volca Midi clock
      if (subClocks[subClockID].run){ // if subclock is running
        //Serial.println("volca");
        //midi.sendClock();
        midi.sendRealTime(RealTimeMessage::TimingClock);
        subClocks[subClockID].startMS = currentMicros;
      }   
    break;
  
  }
}


// Reset
void resetClock() {
  Serial.println("reset Clock");
  mainClocks[0].tickMS = mainClocks[0].tempo/mainClocks[0].subTicks;
  mainClocks[0].currentTick = 0;
  mainClocks[0].lastTick = 0;
  mainClocks[0].lastTickTime = micros();
  mainClocks[0].clockStart = 0;
  mainClocks[0].prevClockStart = 0;
  mainClocks[0].elapsedTime = 0;
  currentMicros = micros();
  
  //reset drum steps
  //drumStepPointer = 0; #

  updateClockTempo();
  //Ensure ticksLeft are correctly initialized for the new cycle
  for (int i = 0; i < numSubClocks; i++) {
    //load delay from delay buffer
    subClocks[i].delay = subClocks[i].delayBuffer;
    subClocks[i].ticksLeft = subClocks[i].tick + subClocks[i].delay;  // Reset to tick + delay
    subClocks[i].tickCounter = 0; //reset the clock Counter
    subClocks[i].delay = 0; //clear the delay again as it shall only be used once
  }

  nextClockCycle(true); //true = from reset
}
  