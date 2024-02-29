// checking if all Multiplexers are working


// Pins
const int I1 = 38;    //Analog In -> synth rechts
const int I2 = 41;    //Analog In -> synth links
const int I3 = 40;    //Analog In
const int I4 = 39;    //Analog In
const int I5 = 25;    //Analog In
const int I6 = 26;    //Analog In
const int I7 = 11;    //Digital In
const int I8 = 12;    //Digital Out
const int A = 37;     //Digital Out
const int B = 36;     //Digital Out
const int C = 35;     //Digital Out
const int Aout = 30;  //Digital Out
const int Bout = 31;  //Digital Out
const int Cout = 32;  //Digital Out



// Settings
const int pinPerMux = 8; // 4051 Multiplexer has 8 pins
const int numAnalogMux = 5; // how many analog multiplexers

///
/// Arrays
///

// int arr_prev_read_analog_inputs [numAnalogMux][pinPerMux] {
//   {0, 0, 0, 0, 0, 0, 0, 0}; //I1
//   {0, 0, 0, 0, 0, 0, 0, 0}; //
// };

// previous read value to reduce flickering
int prev_read_I1 [pinPerMux] = {0, 0, 0, 0, 0, 0, 0, 0};
int prev_read_I2 [pinPerMux] = {0, 0, 0, 0, 0, 0, 0, 0};

// current read value
int read_I1 [pinPerMux] = {0, 0, 0, 0, 0, 0, 0, 0};
int read_I2 [pinPerMux] = {0, 0, 0, 0, 0, 0, 0, 0};


// Masks to enable/disable single inputs/outputs
// Used to ignored non-connected inputs/outputs
// 1 = disabled
bool disable_I1 [pinPerMux] = {0, 0, 0, 0, 0, 0, 1, 1};
bool disable_I2 [pinPerMux] = {0, 0, 0, 0, 0, 0, 0, 0};


///
/// Functions
///

void selectMuxInPin(byte pin) {

  bool valA = bitRead(pin, 0);
  bool valB = bitRead(pin, 1);
  bool valC = bitRead(pin, 2);

  digitalWrite(A, valA);
  digitalWrite(B, valB);
  digitalWrite(C, valC);
  delay(1);
}

void selectMuxOutPin(byte pin){
  bool valA = bitRead(pin, 0);
  bool valB = bitRead(pin, 1);
  bool valC = bitRead(pin, 2);

  digitalWrite(Aout, valA);
  digitalWrite(Bout, valB);
  digitalWrite(Cout, valC);
  delay(1);
}


void readAnalogPins() {
  
  // read multiplexer
  for (byte pin=0; pin<=7; pin++){
    selectMuxInPin(pin);
    int raw_I1;
    if (!disable_I1[pin]){
      raw_I1 = analogRead(I1);
      prev_read_I1[pin] = read_I1[pin];
      read_I1[pin] = map(raw_I1,0,1023,0,127);
    }
    
    if (prev_read_I1[pin] != read_I1[pin]) {
      Serial.print(pin);
      Serial.print(" reads: ");
      Serial.println(raw_I1);
      usbMIDI.sendControlChange(1 + pin, read_I1[pin], 1);
    }



    int raw_I2;
    if (!disable_I2[pin]){
      raw_I2 = analogRead(I2);
      prev_read_I2[pin] = read_I2[pin];
      read_I2[pin] = map(raw_I2,0,1023,0,127);
    }
    
    if (prev_read_I2[pin] != read_I2[pin]) {
      Serial.print(pin);
      Serial.print(" reads: ");
      Serial.println(raw_I2);
      usbMIDI.sendControlChange(10 + pin, read_I1[pin], 1);
    }
  }
}

void readDigitalPins() {
  
  // read multiplexer
  for (byte pin=0; pin<=7; pin++){
    selectMuxInPin(pin);
    bool readRaw = digitalRead(I7);
    //muxValues[pin] = map(seqRaw,0,1023,0,85);
    Serial.print(pin);
    Serial.print(" reads: ");
    Serial.println(readRaw);
  }
}


void setup() {
  pinMode(I1, INPUT);
  pinMode(I2, INPUT);
  //pinMode(I3, INPUT);
  //pinMode(I4, INPUT);
  //pinMode(I5, INPUT);
  //pinMode(I6, INPUT);
  //pinMode(I7, INPUT_PULLUP);
  //pinMode(I8, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(Aout, OUTPUT);
  pinMode(Bout, OUTPUT);
  pinMode(Cout, OUTPUT);
}

void loop() {
  readAnalogPins();
}
