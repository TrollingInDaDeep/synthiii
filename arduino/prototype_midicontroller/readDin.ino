/*
 * Reads analog value (Potentiometers)
 * sends it via midi to designated ControlChange
 */

void readDin(int currentValue, inputNr)
 {

    
    sendMIDI(statusByteSetDigitalValue, currentValue, inputNr);
 }
