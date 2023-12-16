/*
 * Reads analog value (Potentiometers)
 * sends it via midi to designated ControlChange
 */

void readAin(int currentValue, int inputNr)
 {

    int analogDiff = abs(currentValue - analogInputs[inputNr]);
    if (( analogDiff > 0 && analogDiff >= analogChangeThreshold))
    {
      analogInputs[inputNr] = currentValue;
      //Serial.print(inputNr);
      //Serial.print(":");
      //Serial.println(analogInputs[inputNr]);
      //176 means controlChange Channel 1
      
      
      sendMIDI(statusByteAin, inputNr, analogInputs[inputNr]);
      //Serial.print(inputNr);
      //Serial.print(": ");
      //Serial.println(currentValue);

    }
  
 }
