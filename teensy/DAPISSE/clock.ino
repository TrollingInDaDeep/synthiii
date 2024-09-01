// reading the code of clocked vcv rack module
// https://github.com/MarcBoule/ImpromptuModular/blob/master/src/Clocked.cpp#L652
// realizing a separate master clock is a way better idea
// clocked is a great module


///Variables




//external clock handler
//tap tempo


// subclocks



// connect subclocks to
 // a -> the sequencer
 // b -> drum instrument 1
 // c -> drum instrument 2

///
/// Functions
///


// updateClock -> run several times also during digital Read
// check if any action to do (tick or subtick)
// update subticks
void updateClock() {
  currentMillis = millis();
  currentTick = (currentMillis - clockStart) / tickMS; //division with int will only yield whole numbers.
  updateClockTempo();



  // Serial.print("millis:");
  // Serial.print(currentMillis);
  // Serial.print(" preTickSTart:");
  // Serial.println(prevTickStart);
  //Serial.print(" prevClockStart:");
  //Serial.println(prevClockStart);
  if (currentMillis - tickStart >= tickMS) {
    // Serial.print(currentMillis - tickStart);
    // Serial.print(">=");
    // Serial.println(tickMS);
    nextTick();
  }
  
  if (currentMillis - clockStart >= tempo) {
    // Serial.print(millis() - prevClockStart);
    // Serial.print(">=");
    // Serial.println(tempo);
    nextClockCycle();
  }
}

//Run actions needed for next Tick
void nextTick() {
  prevTickStart = tickStart;
  tickStart = currentMillis;
  Serial.print("t: ");
  Serial.println(currentTick);

  for (int i = 0; i < numSubClocks; i++){
    if (subClocks[i][8] > 0){// don't decrease when already 0, don't kick downwards pls
          subClocks[i][5]--; //decrease the ticks left
    }
    if (subClocks[i][8] == 1){ // if subclock is running
      Serial.print("Ticks left: ");
      Serial.println(subClocks[i][5]);
    }
    if (subClocks[i][5] < 1) { // No ticks left -> trigger!
      if (subClocks[i][8] == 1){ // if subclock is running
        Serial.print("$$$ NO Ticks left: ");
        Serial.println(subClocks[i][5]);
      }
      clockHandler(i);
      //reset tick counter for subclock. Tick + delay
      subClocks[i][5] = subClocks[i][3] + subClocks[i][4];
      if (subClocks[i][8] == 1){ // if subclock is running
        Serial.print("Reset to: ");
        Serial.println(subClocks[i][5]);
      }
    }
  }
  

  // Serial.print("tick: ");
  // Serial.println(currentMillis);
}

//run actions needed for next Clock Cycle (beat)
void nextClockCycle() {
  prevClockStart = clockStart;
  clockStart = currentMillis;
  Serial.println("clock: ");
  usbMIDI.sendClock();
  // Serial.println(millis());
}



//update tempo
/// sets the tempo modifier of the subclock
/// expects number to multiply or divide clock
void updateClockTempo() {
  //bug: might lead to always start at 2nd tick
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
    Serial.println(subClocks[0][3]);
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
      nextPulse();
      Serial.println("pulse Sequencer");
    }
  }
  
}

 // reset