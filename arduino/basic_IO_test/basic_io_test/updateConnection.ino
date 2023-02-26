/*
 * Handles updates in connections
 * Status of every possible connecion is stored in
 * arrConnectionStates [][] -> defined in main file (prototype_midicontroller)
 *  
 */

/// updates connection status in pre-stored array
/// afterwards sends connection update via midi
void updateConnection(int Cin, int Cout, bool blnConnected)
 {
        
        Serial.print(arrConnectionStates[Cin][Cout]);
        Serial.print("<-arr  bln->");
        Serial.println(blnConnected);
        // Serial.print(";");
        // Serial.println(blnConnected);
        
  if (arrConnectionStates[Cin][Cout] != blnConnected)
    {
      arrConnectionStates[Cin][Cout] = blnConnected;

      //connect if true, disconnect if false
      // todo: define how this actually works
      if (blnConnected)
      {
        //sendMIDI(statusByteMakeConnection, Cin, Cout);
        Serial.print("Connected ");
        Serial.print(Cout);
        Serial.print(" to ");
        Serial.println(Cin);
      }
      else
      {
        //sendMIDI(statusByteBreakConnection, Cin, Cout);
        Serial.print("Disconnected ");
        Serial.print(Cout);
        Serial.print(" from ");
        Serial.println(Cin);
      }
      
    }
    delay(1000);
 }
