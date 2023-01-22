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
  //const int Dout = 2;
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

// many pins are connected to a L1 mux
// how to connect stuff to a Mux (L1):
// add a mux to the first outputs in order: 1. output 0, 2. output 1 etc.
// afterwards add direct connections on the remaining pins.
// that way, we can loop through the first # pins and also loop through T2 as there's a mux to read
// if we're above #, we only need to read the value once, as there's no mux connected to T2
//alternatively put:
// Multiplexers on T2 are added output by output (ascending) on the multiplexers on T1
// we only want to loop through T2 when any of the T1 Mux has a T2 connected to this output

const int intMuxAin = 1 // means one L2 multiplexer is connected to mux Ain (always on output0, then output1, etc.)
const int intMuxDin = 1
const int intMuxCout = 1
const int intMuxCin = 1

// combine the values to one mux. only used to limit T2 iterations.
//maximum value from the 4
int maxMux = max4(intMuxAin, intMuxDin, intMuxCout, intMuxCin) // yes the var is called maxMux :)


loop until max. value above
on data entry and sending -> loop only until max. value of the type (din/ain/cout/cin)

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
// first value is Cin index , 2nd value is Cout index
  bool arrConnectionStates[][];

// var declarations to use later
// stores current state/value of inputs/outputs/mux control pins
  int intCurrAin;
  bool blnCurrDin;
  bool blnCurrCin;
  bool blnCurrCout;
  bool blnCurrT3_A;
  bool blnCurrT3_B;
  bool blnCurrT3_C;
  bool blnCurrT2_A;
  bool blnCurrT2_B;
  bool blnCurrT2_C;
  bool blnCurrT1_A;
  bool blnCurrT1_B;
  bool blnCurrT1_C;


  int intInputNr; // which input are we on
  int intOutputNr; // which output are we on

void setup() {
  //set pin modes
  //Analog
  pinMode(A0, INPUT);  //Ain
  pinMode(A3, OUTPUT); //T4_A
  pinMode(A4, OUTPUT); //T4_B
  pinMode(A5, OUTPUT); //T4_C

  //Digital
  pinMode(Din,INPUT_PULLUP);
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
  //for(int i=0;i<nrCout;i++){
    //for(int j=0;j<nrCin;i++){
      //  arrConnectionStates[i][j]=0;
    //}
//}
  
}

void loop() {

//reset input/output indexes
intInputNr = 0;
intOutputNr = 0;

//set Cout pin high
digitalWrite(Cout, HIGH);

// outer loop: through T3 (Cout L1)
for(int T3=0;T3<8;T3++){
        intOutputNr++;

        //use T3 for Cout L1
        blnCurrT3_A = bitRead(T3, 0); //LSB
        blnCurrT3_B = bitRead(T3, 1);
        blnCurrT3_C = bitRead(T3, 2); //MSB
        digitalWrite(T3_A, blnCurrT3_A);
        digitalWrite(T3_B, blnCurrT3_B);
        digitalWrite(T3_C, blnCurrT3_C);        


        // loop through T4 (Cout L2)
        for(int T4=0;T4<8;T4++){
            intOutputNr++;
            //use T4 for Cout L2
            blnCurrT4_A = bitRead(T4, 0); //LSB
            blnCurrT4_B = bitRead(T4, 1);
            blnCurrT4_C = bitRead(T4, 2); //MSB
            digitalWrite(T4_A, blnCurrT4_A);
            digitalWrite(T4_B, blnCurrT4_B);
            digitalWrite(T4_C, blnCurrT4_C);

            //inner loop: through T1. Ain, Din, Cin L1
            for(int T1=0;T1<8;T1++){
                intInputNr++;

                //use T1 for Ain, Din, Cin L1
                blnCurrT1_A = bitRead(T1, 0); //LSB
                blnCurrT1_B = bitRead(T1, 1);
                blnCurrT1_C = bitRead(T1, 2); //MSB
                digitalWrite(T1_A, blnCurrT1_A);
                digitalWrite(T1_B, blnCurrT1_B);
                digitalWrite(T1_C, blnCurrT1_C);

               //loop through T2. Ain, Din, Cin L2
                for(int T2=0;T2<8;T2++){
                      intInputNr++;

                      //use T2 for Ain, Din, Cin L2
                      blnCurrT2_A = bitRead(T2, 0); //LSB
                      blnCurrT2_B = bitRead(T2, 1);
                      blnCurrT2_C = bitRead(T2, 2); //MSB
                      digitalWrite(T2_A, blnCurrT2_A);
                      digitalWrite(T2_B, blnCurrT2_B);
                      digitalWrite(T2_C, blnCurrT2_C);

                      //do actual stuff

                      //read cin
                      blnCurrCin = digitalRead(Cin);
                      // set connection at intInputNr:intOutputNr to blnCurrCin
                      updateConnection(intInputNr, intOutputNr, blnCurrCin)

                      // Read analog value and send it to Analog function.
                      intCurrAin = analogRead(Ain);
                      readAin(intCurrAin, intInputNr);
                      

                      // Read Digital value and send it to Digital function.
                      blnCurrDin = digitalRead(Din)
                      readDin(blnCurrDin, intInputNr);

                }
            }
        }
    }


              
              //read Ain, write value to Analog T1:T2

              //read Din, write value to Digital T1:T2






  // false loop:
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

}

void sendMIDI(byte statusByte, byte dataByte1, byte dataByte2) {
 Serial.write(statusByte);
 Serial.write(dataByte1);
 Serial.write(dataByte2);
}

// gives back the maximum out of 4 values
int max4(int a, int b, int c, int d)
{
   int maxguess;

   maxguess = max(a,b);  // biggest of a and b
   maxguess = max(maxguess, c);  // but maybe c is bigger?
   maxguess = max(maxguess, d);  // but maybe d is bigger?

   return(maxguess);
}
