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
  if (arrConnectionStates[Cin][Cout] != blnConnected)
    {
      arrConnectionStates[Cin][Cout] == blnConnected;

      //connect if true, disconnect if false
      // todo: define how this actually works
      if (blnConnected)
      {
        sendMIDI(statusByteMakeConnection, Cin, Cout);
        //Serial.println("Connection made");
      }
      else
      {
        sendMIDI(statusByteBreakConnection, Cin, Cout);
        //Serial.println("Connection broken");
      }
      
    }
 }
