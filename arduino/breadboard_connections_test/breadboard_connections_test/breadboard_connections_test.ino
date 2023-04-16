// 25.03.2023
// Dominik Rieder
// https://github.com/TrollingInDaDeep/synthiii
// thanks: https://www.hackster.io/danisferreira27/build-an-arduino-powered-midi-controller-7d04dd
// Sketch for breadboard v1 tests connections of outputs to inputs
// Arduino shield, currently with 2 CD4051BE multiplexers

// Detailed pinout:
// Mux 1 (outgoing connections)
// I/O -> 
// C   -> 4
// B   -> 3
// A   -> 2

// MUX2 (incoming connections)
// I/O -> A4
// C   -> 7
// B   -> 6
// A   -> 5

// Analog Pins
//  const int Ain = A4;
//  const int Ain2 = A5;

// Digital Pins
  //const int Din = 6;
  const int Cout = 8;
  const int Cin = 9;
  const int T1_A = 2;
  const int T1_B = 3;
  const int T1_C = 4;
  const int T2_A = 5;
  const int T2_B = 6;
  const int T2_C = 7;

// var declarations to use later
// stores current state/value of inputs/outputs/mux control pins
  int intCurrAin;
  bool blnCurrDin;
  bool blnCurrCout;
  bool blnCurrCin;
  bool blnCurrT1_A;
  bool blnCurrT1_B;
  bool blnCurrT1_C;
  bool blnCurrT2_A;
  bool blnCurrT2_B;
  bool blnCurrT2_C;

//stores digital values. last read and new read.
  const int numCout = 8;
  const int numCin = 8;
  bool connectionMatrix[numCout][numCin];
  int analogChangeThreshold = 3; // threshold how big a change in pot value needs to be to be sent (smoothening)

  // Status Bytes
  const int statusByteAin = 176; // Status byte for analog inputs. 176 = controlChange Channel 1
  const int statusByteDin = 177; // Status byte for digital inputs. 177 = controlChange Channel 2
  const int statusByteMakeConnection = 144; // 144 = Channel 1 note on
  const int statusByteBreakConnection = 128; // 128 = Channel 1 note off


  int intInputNr; // which input are we on

// Sends midi signal via USB, duh
void sendMIDI(byte statusByte, byte dataByte1, byte dataByte2) {
  Serial.write(statusByte);
  Serial.write(dataByte1);
  Serial.write(dataByte2);
}


void setup() {
  //set pin modes
  //Analog
  //pinMode(Ain, INPUT);
  //pinMode(Ain2, INPUT);

  //Digital
  //pinMode(Din,INPUT_PULLUP);
  pinMode(Cin,INPUT_PULLUP);
  pinMode(Cout,OUTPUT);
  pinMode(T1_A,OUTPUT);
  pinMode(T1_B,OUTPUT);
  pinMode(T1_C,OUTPUT);
  pinMode(T2_A,OUTPUT);
  pinMode(T2_B,OUTPUT);
  pinMode(T2_C,OUTPUT);

  Serial.begin(115200);
  delay(10);
}

void loop() {
  // digitalWrite(T2_A, 0);
  // digitalWrite(T2_B, 0);
  // digitalWrite(T2_C, 0);
  // delay(100);
  // intCurrAin = analogRead(Ain);
  // Serial.println(intCurrAin);

  // delay(500);

  //reset input/output indexes
  //intInputNr = 0;

  for(int opt=0;opt<numCout;opt++){

    //use T1 for Ain L1
    blnCurrT1_A = bitRead(opt, 0); //LSB
    blnCurrT1_B = bitRead(opt, 1);
    blnCurrT1_C = bitRead(opt, 2); //MSB
    digitalWrite(T1_A, blnCurrT1_A);
    digitalWrite(T1_B, blnCurrT1_B);
    digitalWrite(T1_C, blnCurrT1_C);

    digitalWrite(Cout, LOW); // pull to ground as input is pulled high with pullup

    for(int inpt=0;inpt<numCin;inpt++){
      blnCurrT2_A = bitRead(inpt, 0); //LSB
      blnCurrT2_B = bitRead(inpt, 1);
      blnCurrT2_C = bitRead(inpt, 2); //MSB
      digitalWrite(T2_A, blnCurrT2_A);
      digitalWrite(T2_B, blnCurrT2_B);
      digitalWrite(T2_C, blnCurrT2_C);
      delay(1);

      blnCurrCin = digitalRead(Cin);
      delay(1);
      bool blnCheckCin = digitalRead(Cin);
      if (blnCurrCin==blnCheckCin){ //check if value is stable over 1ms, only then send an update
        readDin(!blnCurrCin,opt,inpt); //inverted as inverted logic
      }
    }
    // Read analog value and send it to Analog function.
    
    //readAin(map(intCurrAin,0,1023,0,127), intInputNr); //map 1023 to 127 values for midi
    // Serial.print(intInputNr);
    // Serial.print(":");
    // Serial.println(analogInputs[intInputNr]);
    //Serial.print("\t");
    //Serial.println("");
    //intInputNr++;
  }
}
