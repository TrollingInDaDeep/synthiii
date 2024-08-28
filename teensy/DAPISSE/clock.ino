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

 // divide / multiply

 //trigger actions for each subclock
 //either seq step
 //or drum instrument trigger
 //or note stop -> maybe should be handled via gate time of each instrument by itself


 // reset