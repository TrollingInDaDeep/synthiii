// 25.03.2023
// Dominik Rieder
// https://github.com/TrollingInDaDeep/synthiii
// Sketch for Shield_v1
// Arduino shield, currently with 2 CD4051BE multiplexers
// Used to prototype basic multiplexer connections, still easily removable and all with connector cables
// getting pretty professional eh? 
// One MUX is connected to A0 and one to digital port 6 to have both types of connections

// Detailed pinout:
// Mux 1
// I/O -> A0
// C   -> 0
// B   -> 1
// A   -> 2

// MUX2
// I/O -> 6
// C   -> 3
// B   -> 4
// A   -> 5

// Analog Pins
  const byte Ain = A0;


// Digital Pins
  const int Din = 6;
  const int T1_A = 2;
  const int T1_B = 1;
  const int T1_C = 0;
  const int T2_A = 5;
  const int T2_B = 4;
  const int T2_C = 3;

// var declarations to use later
// stores current state/value of inputs/outputs/mux control pins
  int intCurrAin;
  bool blnCurrT1_A;
  bool blnCurrT1_B;
  bool blnCurrT1_C;
  bool blnCurrT2_A;
  bool blnCurrT2_B;
  bool blnCurrT2_C;



  // Status Bytes
  const int statusByteAin = 176; // Status byte for analog inputs. 176 = controlChange Channel 1
  const int statusByteDin = 177; // Status byte for analog inputs. 177 = controlChange Channel 2

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
  pinMode(Ain, INPUT);

  //Digital
  pinMode(Din,INPUT_PULLUP);
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

    
  digitalWrite(T1_A, 0);
  digitalWrite(T1_B, 0);
  digitalWrite(T1_C, 0);
  delay(100);
  intCurrAin = analogRead(A0);
  Serial.println(intCurrAin);
  delay(500);


















  //reset input/output indexes
  //intInputNr = 0;

  // put your main code here, to run repeatedly:
  // for(int T1=0;T1<8;T1++){
  //   intInputNr++;

  //   //use T1 for Ain L1
  //   blnCurrT1_A = bitRead(T1, 0); //LSB
  //   blnCurrT1_B = bitRead(T1, 1);
  //   blnCurrT1_C = bitRead(T1, 2); //MSB
  //   digitalWrite(T1_A, blnCurrT1_A);
  //   digitalWrite(T1_B, blnCurrT1_B);
  //   digitalWrite(T1_C, blnCurrT1_C);
  //   delay(10);
  //   Serial.print(blnCurrT1_C);
  //   Serial.print(blnCurrT1_B);
  //   Serial.print(blnCurrT1_A);

  //   // Read analog value and send it to Analog function.
  //   intCurrAin = analogRead(Ain);
  //   readAin(intCurrAin, intInputNr);
  //   Serial.print("-");
  //   Serial.print(intInputNr);
  //   Serial.print(":");
  //   Serial.println(intCurrAin);
  //   delay(500);
  // }

}
