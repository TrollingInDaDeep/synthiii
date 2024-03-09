#include <Keypad.h>

// PINS
const int I1 = 38;    //Analog In -> synth rechts
const int I2 = 41;    //Analog In -> synth links
const int I3 = 40;    //Analog In -> seq links pot
const int I4 = 39;    //Digital In -> seq links btn
const int I5 = 25;    //Analog In -> seq faders
const int I6 = 26;    //Analog In -> seq pulse
const int I7 = 11;    //Digital Out -> seq LEDs
const int I8 = 12;    //Digital In -> seq btn triggers
const int A = 37;     //Digital Out
const int B = 36;     //Digital Out
const int C = 35;     //Digital Out
const int Aout = 30;  //Digital Out
const int Bout = 31;  //Digital Out
const int Cout = 32;  //Digital Out

//Multiplexers
// Settings
const int pinPerMux = 8; // 4051 Multiplexer has 8 pins
const int numAnalogInMux = 5; // how many analog multiplexers
const int numDigitalInMux = 2; // how many analog multiplexers
const int analogSmoother = 2; // how much does an analog value have to differ to detect change. higher means less jumping but less accurate

//sequencer
//const int note = 42;
// standard velocity for notes
const uint8_t velocity = 127;


///
/// Variable definitions
///

int analogReadDiff = 0;

// ############
// DRUMS
// ############
int midiC = 60; // Standard midi C
int transpose = 0;

// number of instruments (0=kick, 1=snare, 2=highhat, 3=cymbal)
const int drumInstruments = 4;
// which midi note to play for each instrument
const int instrumentNotes[] = {60, 75, 63, 67};
// which instrument is currently selected for keypad play mode
int selectedInstrument = 0;



//number of notes / steps the drum sequencer has
const int numDrumSteps = 16;
// pointer at which step in the drum sequencer we are
int drumStepPointer = 0;

int keypadMode = 0; //0=play, 1=sequence notes 2=fillXStep, 4=settings
bool runDrum = true;

//bools if a drum sound should be triggered at the selected step
bool drumSequence [drumInstruments][numDrumSteps] {
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
};

int howManyKeypadKeys = 16;
char drumPadChars [16] = {'1', '2', '3', '+', '4', '5', '6', '>', '7', '8', '9', '!', '*', '0', '#', 'R'};


// ############
// KEYPAD
// ############
const byte ROWS = 4;
const byte COLS = 4;
//hex because only 1 digit char possible
char hexaKeys[ROWS][COLS] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', 'A', 'B'},
  {'C', 'D', 'E', 'F'}
};

// flachbandkabel:
// weisser leerer pin links
// R1, R2, C1, C2, C3, C4, R3, R4
byte rowPins[ROWS] = {2, 3, 9, 8}; 
byte colPins[COLS] = {4, 5, 6, 7}; 
Keypad kpd = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

/// ############
/// Sequencer
/// ############

int rawAnalog = 0;
bool rawDigital = 0;

// alternative timer
long bpm = 90.0;
long tempo = 1000.0/(bpm/60.0); //bpm in milliseconds

float prevPulseStart = 0; //previous millisecond timestamp before last pulse was started
float prevClockStart = 0; //previous millisecond timestamp before last clock signal was sent
float clockInterval = tempo/24; //number of milliseconds after which to send the midi clock. MIDI spec is to send clock 24 times per quarter note
float pulseStart = 0; //timestamp when the pulse started
float noteStart = 0; //timestamp when the note started



//int clockspeed = 120;
bool run = true;
bool reset = false;
int numSteps = 8; // how many steps should be done. Jumps back to first step after numSteps
const int maxSteps = 8; // Maximum number of Steps of your sequencer
//int stepValues[maxSteps] {0, 0, 0, 0, 0, 0, 0, 0}; // initialize values to middle
//bool digitalInputs1[8] {0, 0, 0, 0, 0, 0, 0, 0}; //first multiplexer with digital inputs
//bool prevDigitalInputs1[8] {0, 0, 0, 0, 0, 0, 0, 0}; //previous state of the inputs

int pulsePointer = 0; //points to the pulse within the step we're currently in
int lastStepPointer = 0; //previous step, to trigger note off
int lastNoteSent = 0; //previous note sent, to trigger note off on time
int pulseCount [] {1, 1, 1, 1, 1, 1, 1, 1}; // how many times a step should be played
int gateMode [maxSteps] {2, 2, 2, 2, 2, 2, 2, 2}; //0=no gate, 1=first gate, 2=every gate
bool skipStep [maxSteps] {0, 0, 0, 0, 0, 0, 0, 0}; //1=skip a step, 0=don't skip

const int numSeqBanks = 4; // how many modes we can have for the sequencer buttons
int selectedSeqBank = 0; // which sequencer bank is selected
// 0=play/trigger pulse/step
// 1=enable/disable skip step
// 2=enable/disable slide step
// 3=?? open for ideas

// stores the different bank values that the sequencer buttons can have
// is int, as first row needs int value
// rest are actually bools
int arr_seq_buttons [numSeqBanks][maxSteps] {
  {0, 0, 0, 0, 0, 0, 1, 1},   // Step Value
  {0, 0, 0, 0, 0, 0, 0, 0},   // Skip Step
  {0, 0, 0, 0, 0, 0, 1, 1},   // Slide Step
  {0, 0, 0, 0, 0, 0, 0, 0}    // ??
};

bool noteStopped = true; //if note has been stopped for this step already

int stepPointer = 0; //pointer, points to current step that we're at
int gateTime = 50; //time in ms how long the note should be on

///
/// End variable definitions
///



///
/// Arrays
///

// collection of analog inputs
int arr_pin_analog_inputs [numAnalogInMux] {
  I1,
  I2,
  I3,
  I5,
  I6
};

// collection of digital inputs
int arr_pin_digital_inputs [numDigitalInMux] {
  I4,
  I8
};

// previous reading of analog inputs
int arr_prev_read_analog_inputs [numAnalogInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 0, 0}, //I1
  {0, 0, 0, 0, 0, 0, 0, 0}, //I2
  {0, 0, 0, 0, 0, 0, 0, 0}, //I3
  {0, 0, 0, 0, 0, 0, 0, 0}, //I5
  {0, 0, 0, 0, 0, 0, 0, 0} //I6
};

// previous reading of digital inputs
int arr_prev_read_digital_inputs [numDigitalInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 0, 0}, //I4
  {0, 0, 0, 0, 0, 0, 0, 0} //I8
};

// current reading of analog inputs
int arr_read_analog_inputs [numAnalogInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 0, 0}, //I1
  {0, 0, 0, 0, 0, 0, 0, 0}, //I2
  {0, 0, 0, 0, 0, 0, 0, 0}, //I3
  {0, 0, 0, 0, 0, 0, 0, 0}, //I5
  {0, 0, 0, 0, 0, 0, 0, 0} //I6
};

// current reading of digital inputs
int arr_read_digital_inputs [numDigitalInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 0, 0}, //I4
  {0, 0, 0, 0, 0, 0, 0, 0} //I8
};

// midi/CC to send for analog inputs
int arr_send_analog_inputs [numAnalogInMux][pinPerMux] {
  {1, 2, 3, 4, 5, 6, 7, 8}, //I1
  {9, 10, 11, 12, 13, 14, 15, 16}, //I2
  {17, 18, 19, 20, 21, 22, 23, 24}, //I3
  {25, 26, 27, 28, 29, 30, 31, 32}, //I5
  {33, 34, 35, 36, 37, 38, 39, 40} //I6
};

// midi/CC to send for digital inputs
int arr_send_digital_inputs [numDigitalInMux][pinPerMux] {
  {41, 42, 43, 44, 45, 46, 47, 48}, //I4
  {49, 50, 51, 52, 53, 54, 55, 56} //I8
};

// Masks to enable/disable single inputs/outputs
// Used to ignored non-connected inputs/outputs
// 1 = disabled
bool arr_disable_analog_inputs [numAnalogInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 1, 1}, //I1
  {0, 0, 0, 0, 0, 0, 0, 0}, //I2
  {0, 0, 0, 0, 1, 1, 1, 1}, //I3
  {0, 0, 0, 0, 0, 0, 0, 0}, //I5
  {0, 0, 0, 0, 0, 0, 0, 0} //I6
};

bool arr_disable_digital_inputs [numDigitalInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 1, 1}, //I4
  {0, 0, 0, 0, 0, 0, 0, 0} //I8
};


// stores the type of input. 0=MIDI CC, 1=internal, value is only used on teensy and not sent as MIDI CC
bool arr_analog_input_type [numAnalogInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 1, 1}, //I1
  {0, 0, 0, 0, 0, 0, 0, 0}, //I2
  {0, 0, 0, 0, 1, 1, 1, 1}, //I3
  {0, 0, 0, 0, 0, 0, 0, 0}, //I5
  {0, 0, 0, 0, 0, 0, 0, 0} //I6
};

bool arr_digital_input_type [numDigitalInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 1, 1}, //I4
  {0, 0, 0, 0, 0, 0, 0, 0} //I8
};

///
/// Functions
///

//Function Prototypes
void nextDrumStep(void);
int youFuckenKiddingMe(char);
void startNote(int);
void stopNote(int);



// what happens when a clock signal is received
void handleClock() {
  if (runDrum) {
    nextDrumStep();
  }
}
// selects the pin on input multiplexer
void selectMuxInPin(byte pin) {

  bool valA = bitRead(pin, 0);
  bool valB = bitRead(pin, 1);
  bool valC = bitRead(pin, 2);

  digitalWrite(A, valA);
  digitalWrite(B, valB);
  digitalWrite(C, valC);
  delay(1);
}

// selects the pin on output multiplexer
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
  // loop through analog in multiplexers
  for (int mux = 0; mux<numAnalogInMux; mux++){
    
    //loop through every pin for the analog multiplexers
    for (byte pin=0; pin<pinPerMux; pin++){
        selectMuxInPin(pin);
        // only read if input is not disabled
        if (!arr_disable_analog_inputs[mux][pin]){
          rawAnalog = analogRead(arr_pin_analog_inputs[mux]);
          arr_prev_read_analog_inputs[mux][pin] = arr_read_analog_inputs[mux][pin];
          arr_read_analog_inputs[mux][pin] = map(rawAnalog,0,1023,0,127);
        }
        
        
      }
  }
}

void readDigitalPins() {
  // loop through digital in multiplexers
  for (int mux = 0; mux<numDigitalInMux; mux++){
    
    //loop through every pin for the digital muxes
    for (byte pin=0; pin<pinPerMux; pin++){
        selectMuxInPin(pin);
        // only read if input is not disabled
        if (!arr_disable_digital_inputs[mux][pin]){
          rawDigital = digitalRead(arr_pin_digital_inputs[mux]);
          arr_prev_read_digital_inputs[mux][pin] = arr_read_digital_inputs[mux][pin];
          arr_read_digital_inputs[mux][pin] = rawDigital;
        }
    }
  }
}


void UpdateSendValues() {
  
  ///
  ///Analog
  ///
  for (int mux = 0; mux<numAnalogInMux; mux++){


    //loop through every pin for the analog multiplexers
    for (byte pin=0; pin<pinPerMux; pin++){
      //calculate difference of the read values. square and squareroot to always get positive difference
      analogReadDiff = arr_prev_read_analog_inputs[mux][pin] - arr_read_analog_inputs[mux][pin];
      analogReadDiff = sq(analogReadDiff);
      analogReadDiff = sqrt(analogReadDiff);

      if (analogReadDiff > analogSmoother) {
        // Serial.print("mux ");
        // Serial.print(mux);
        // Serial.print(" pin ");
        // Serial.print(pin);
        Serial.print("Controller ");
        Serial.print(arr_send_analog_inputs[mux][pin]);
        Serial.print(": ");
        Serial.println(arr_read_analog_inputs[mux][pin]);

        // Type = MIDI CC
        if (!arr_analog_input_type[mux][pin]) {
          usbMIDI.sendControlChange(arr_send_analog_inputs[mux][pin], arr_read_analog_inputs[mux][pin], 1);
        } else {
          //Type = Internal Variable
          //Mapping of Controller Number (from arr_send_analog_inputs) to internal variables
          switch (arr_send_analog_inputs[mux][pin]){
            case 1:


            break;
            case 2:

            break;
            case 3:

            break;
            case 4:

            break;
            case 5:

            break;
            case 6:

            break;
            case 7:

            break;
            case 8:

            break;
            case 9:

            break;
            case 10:

            break;
            case 11:

            break;
            case 12:

            break;
            case 13:

            break;
            case 14:

            break;
            case 15:

            break;
            case 16:

            break;
            case 17:

            break;
            case 18:

            break;
            case 19:

            break;
            case 20:

            break;
            case 21:

            break;
            case 22:

            break;
            case 23:

            break;
            case 24:

            break;
            case 25:

            break;
            case 26:

            break;
            case 27:

            break;
            case 28:

            break;
            case 29:

            break;
            case 30:

            break;
            case 31:

            break;
            case 32:

            break;
            case 33:

            break;
            case 34:

            break;
            case 35:

            break;
            case 36:

            break;
            case 37:

            break;
            case 38:

            break;
            case 39:

            break;
            case 40:

            break;

          }
        }
      }
    }   
  }

  ///
  ///Digital
  ///
  for (int mux = 0; mux<numDigitalInMux; mux++){
    //loop through every pin for the analog multiplexers
    for (byte pin=0; pin<pinPerMux; pin++){
      if (arr_prev_read_digital_inputs[mux][pin] != arr_read_digital_inputs[mux][pin]) {
        Serial.print("Digital ");
        Serial.print(arr_send_digital_inputs[mux][pin]);
        Serial.print(": ");
        Serial.println(arr_read_digital_inputs[mux][pin]);


        // Type = MIDI CC
        if (!arr_digital_input_type[mux][pin]) {
          //usbMIDI.sendControlChange(arr_send_digital_inputs[mux][pin], arr_read_digital_inputs[mux][pin], 1);
        } else {
        //Type = Internal Variable
        //Mapping of Controller Number (from arr_send_digital_inputs) to internal variables
        switch (arr_send_analog_inputs[mux][pin]){
            case 1:
              

            break;
            case 2:

            break;
            case 3:

            break;
            case 4:

            break;
            case 5:

            break;
            case 6:

            break;
            case 7:

            break;
            case 8:

            break;
            case 9:

            break;
            case 10:

            break;
            case 11:

            break;
            case 12:

            break;
            case 13:

            break;
            case 14:

            break;
            case 15:

            break;
            case 16:

            break;
            case 17:

            break;
            case 18:

            break;
            case 19:

            break;
            case 20:

            break;
            case 21:

            break;
            case 22:

            break;
            case 23:

            break;
            case 24:

            break;
            case 25:

            break;
            case 26:

            break;
            case 27:

            break;
            case 28:

            break;
            case 29:

            break;
            case 30:

            break;
            case 31:

            break;
            case 32:

            break;
            case 33:

            break;
            case 34:

            break;
            case 35:

            break;
            case 36:

            break;
            case 37:

            break;
            case 38:

            break;
            case 39:

            break;
            case 40:

            break;

          }
      }
    }
  }


}

void setup() {
  pinMode(I1, INPUT);
  pinMode(I2, INPUT);
  pinMode(I3, INPUT);
  pinMode(I4, INPUT_PULLUP);
  pinMode(I5, INPUT);
  pinMode(I6, INPUT);
  //pinMode(I7, OUTPUT);
  pinMode(I8, INPUT_PULLUP);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(Aout, OUTPUT);
  pinMode(Bout, OUTPUT);
  pinMode(Cout, OUTPUT);


  usbMIDI.setHandleClock(handleClock);
}

void loop() {
  //reading drumpad after every function for lower latency
  readDrumpad();
  readAnalogPins();

  readDrumpad();
  readDigitalPins();

  readDrumpad();
  usbMIDI.read();

  UpdateSendValues();
}
