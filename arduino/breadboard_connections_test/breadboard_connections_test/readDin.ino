/*
 * Reads analog value (Potentiometers)
 * sends it via midi to designated ControlChange
 */

void readDin(bool currentValue, int optNr, int inptNr)
 {

    int currentValueDec; //value will be sent in decimal. 0 for false, 127 for true
    if(( connectionMatrix[optNr][inptNr] != currentValue )){
      connectionMatrix[optNr][inptNr] = currentValue;

        if (connectionMatrix[optNr][inptNr]) //
        {
          currentValueDec = 127;
          // Serial.print(optNr);
          // Serial.print("->");
          // Serial.println(inptNr);

          sendMIDI(statusByteMakeConnection, optNr, inptNr);
        } else
        {
          currentValueDec = 0;
          // Serial.print(optNr);
          // Serial.print(" | ");
          // Serial.println(inptNr);

          sendMIDI(statusByteBreakConnection, optNr, inptNr);
        }
      }
    }

