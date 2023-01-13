/*
 * Prototype Board for Arduino midi controller
 *  
 *  14.12.2022
 *  Dominik Rieder
 *  https://github.com/TrollingInDaDeep/synthiii
 *   
 * Sketch controls 4 Multiplexers 8CD4051BE)
 * Each Multiplexer has another (cascaded) multiplexer connected to Pin13 (Connector 0) -> this can be expanded to 1 -> 8 Multiplexers
 * First row of Multiplexers are addressed by T1 (A/B/C)
 * Second row of Multiplexers are addressed by T2 (A/B/C)
 * 
 * 
 * Thanks to:
 * https://www.goetzmd.de
 * https://www.midi.org/specifications-old/item/table-2-expanded-messages-list-status-bytes
 * https://www.midi.org/specifications-old/item/table-3-control-change-messages-data-bytes-2
 * 
 */

 
///
/// constant variables (settings)
///

// Analog Pins
  const int Ain = 0;
  //A1 -> unused
  //A2 -> unused
  const int T4_A = 3;
  const int T4_B = 4;
  const int T4_C = 5;

// Ditital Pins
  const int Din = 1;
  const int Dout = 2;
  const int Cin = 3;
  const int Cout = 4;
  const int T3_A = 5;
  const int T3_B = 6;
  const int T3_C = 7;
  const int T2_A = 8;
  const int T2_B = 9;
  const int T2_C = 10;
  const int T1_A = 11;
  const int T1_B = 12;
  const int T1_C = 13;


// Arrays for Items
// 9 Analog inputs
int arrAin[9];

// 1 Digital inputs (switch)
bool arrDin[1];

// 5 Connection Outputs
bool arrCout[5];

// 9 connection inputs
bool arrCin[9];
// Max. Number of connections on T1-4?
// what about empty ones?

// Number of Connection inputs and outputs
const int nrCout = 5;  //number of connection outputs
const int nrCin = 9;   // number of connection inputs

//Defines the statusByte that should be used by the updateConnection method
const byte statusByteMakeConnection = 145; //means Channel 2 Note on
const byte statusByteBreakConnection = 130; //means Channel 2 Note off


///
/// variables (pre-declarations)
///

// Array that holds all connection states
// first value is Cout index , 2nd value is Cin index
bool arrConnectionStates[nrCout][nrCin];

void setup() {
  // set pin modes
  //Analog
  pinMode(A0, INPUT);
  pinMode(A3, OUTPUT); //T4_A
  pinMode(A4, OUTPUT); //T4_B
  pinMode(A5, OUTPUT); //T4_C

  //Digital
  pinMode(Din,INPUT_PULLUP);
  pinMode(Dout,OUTPUT);
  pinMode(Cin,INPUT_PULLUP);
  pinMode(Cout,OUTPUT);
  pinMode(T3_A,OUTPUT);
  pinMode(T3_B,OUTPUT);
  pinMode(T3_C,OUTPUT);
  pinMode(T2_A,OUTPUT);
  pinMode(T2_B,OUTPUT);
  pinMode(T2_C,OUTPUT);
  pinMode(T1_A,OUTPUT);
  pinMode(T1_B,OUTPUT);
  pinMode(T1_C,OUTPUT);


  //find longest array for T3
  // initialize connection values all to 0
  for(int i=0;i<nrCout;i++){
    for(int j=0;j<nrCin;i++){
        arrConnectionStates[i][j]=0;
    }
}
  
}

void loop() {


// check empty pins
//check double pins where its connected to T1 instead of T2

  // set T1 to 0



// loop through Output connections and apply voltage
For (output = 0; output < sizeof(arrCout); output++)
{
    //todo: set multiplexer T3 to output
    digitalWrite(Cout, HIGH)
}



  // set T1-4 to 0
  int T1 = 0;
  int T2 = 0;
  int T3 = 0;
  int T4 = 0;


//invert the loop: inner loop should be sensor readings, outer loop connection readings

  
  // loop through T1
      
      // loop through T2
          //read Ain
          // write to array
          //read Din
          // write to array
          // sendmidi
            // loop through T3
              //set pin high
              
                  // loop through T4
                    //read cin
                        //todo calculate cin/cout number
                        bool blnConnected = digitalRead(Cin);
                        //updateConnection(iCin, iCout, 
                        
                    // write cin state to array
                    // sendmidi


  // sendMidi all (oooor, see above)
                  

}

void sendMIDI(byte statusByte, byte dataByte1, byte dataByte2) {
 Serial.write(statusByte);
 Serial.write(dataByte1);
 Serial.write(dataByte2);
}
