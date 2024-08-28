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
  Serial.println(currentTick);

  for (int i = 0; i < numSubClocks; i++){
    subClocks[i][5]--; //decrease the ticks left
    
    if (subClocks[i][5]) { // No ticks left -> trigger!
      clockHandler(i);
      //reset tick counter for subclock. Tick + delay
      subClocks[i][5] = subClocks[i][3] + subClocks[i][4];
    }
  }
  

  // Serial.print("tick: ");
  // Serial.println(currentMillis);
}

//run actions needed for next Clock Cycle (beat)
void nextClockCycle() {
  prevClockStart = clockStart;
  clockStart = currentMillis;
  // Serial.print("clock: ");
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
    if (subClocks[2][i] == 1){
      subClocks[3][i] = numTicks / subClocks[1][i]; // ticks = numTicks / ratio
    }
    //Multiplication of tempo
    else
    {
      subClocks[3][i] = numTicks * subClocks[1][i]; // ticks = numTicks * ratio
    }
  }
}

 //trigger actions for each subclock
 //either seq step
 //or drum instrument trigger
 //or note stop -> maybe should be handled via gate time of each instrument by itself
void clockHandler (int subClockID) {
  case 0: // sequencer
    if (subClocks[subClockID][8] == 1){ // if subclock is running
      nextPulse();
      Serial.println("pulse+++");
    }
}

 // reset