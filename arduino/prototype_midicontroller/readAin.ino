/*
 * Reads analog value (Potentiometers)
 * sends it via midi to designated ControlChange
 */

void readAin(int currentValue, inputNr)
 {

    
    sendMIDI(statusByteCC#, currentValue, inputNr);
 }
