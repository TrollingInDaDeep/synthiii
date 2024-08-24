#include <Keypad.h>
#include <ArduinoTapTempo.h>

///
/// How to
///
/// add multiplexer: add const int under pins, add input/output in setup, add to pin array, add line to all digi OR analog arrays in array section, 
/// set if some inputs/outputs are disabled/not scanned in arr_disable
/// set if MidiCC or internal control
/// say what happens in UpdateSendValues()

// change settings



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
const int digitalSmoother = 0; // how many iterations a digital input should not be updated

//sequencer
//const int note = 42;
// standard velocity for notes
const uint8_t velocity = 127;
int gateTime = 50; //time in ms how long the note should be on
int gateMin = 5; //minimum gate time in ms for pot selection
int gateMax = 1000; //maximum gate time in ms for pot selection
int slideAmount = 0; //how much note slide if enabled MidiCC value from 0-127
int minSeqNote = 20; //minimal Midi Note of sequencer fader
int maxSeqNote = 80; //minimal Midi Note of sequencer fader
int minPulse = 1; //how many pulses at least for sequencer
int maxPulse = 8; //how many pulses at max for sequencer
int slowReadCycleCount = 0;
bool isSlowReadCycle = 1; //set to 1 if we have a slow read cycle (also read slow buttons)
bool syncSequencerToClock = 0; // 1 = next sequencer note triggered on ext clock signal | 0 = internal bpm used as step tempo
bool syncDrumToSequencer = 1; //1 = drum step triggered when sequencer steps | 0 = triggered when external clock received
int synthMidiChannel = 2;
bool blnTapTempo = 1; //1 = playMode button becomes tapTempo button 
///
/// Variable definitions
///
ArduinoTapTempo tapTempo;

int analogReadDiff = 0;
int caseNumber = 0;
int noteNumberDigiRead = 0;
int noteNumberAnalog = 0;
unsigned long currentMillis = 0;

//for benchmark purposes
int startLoopMillis = 0;
int drumPadMillis = 0;
int analogReadMillis = 0;
int digitalReadMillis = 0;
int updateValueMillis = 0;
int seqNotesMillis = 0;
int endLoopMillis = 0;



// ############
// DRUMS
// ############
int midiC = 60; // Standard midi C
int transpose = 0;
float tempoModifier = 6; //multiplication value with sequencer tempo to get n-times the tempo of the sequencer
// number of instruments (0=kick, 1=snare, 2=highhat, 3=cymbal)
const int numDrumInstruments = 4;
// which midi note to play for each instrument
const int instrumentNotes[numDrumInstruments] = {60, 61, 62, 63}; //60=C, 64 = E, 63 = D#, 67 = G
// which instrument is currently selected for keypad play mode
int selectedInstrument = 1;
int drumMidiChannel = 3;
bool drumNoteStopped = true; //if drum notes have been stopped for this step already (goes for all drum notes)
float drumNoteStart = 0; //timestamp when the drum note started
int drumGateTime = 2; //time in ms how long the drum note should be on

//number of notes / steps the drum sequencer has
const int numDrumSteps = 16;
// pointer at which step in the drum sequencer we are
int drumStepPointer = 0;

int keypadMode = 0; //experiment: just use play and add recordDrum switch
                    //originally: 0=play, 1=sequence notes 2=fillXStep, 4=settings
bool runDrum = false;
bool recordDrum = true; // true = played notes are recorded into the drumSequence
//bools if a drum sound should be triggered at the selected step
bool drumSequence [numDrumInstruments][numDrumSteps] {
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, //kick
  //{0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, // snare
  //{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // hat
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1} // ?
};

const int howManyKeypadKeys = 16;
char drumPadChars [howManyKeypadKeys] = {'1', '2', '3', '+', '4', '5', '6', '>', '7', '8', '9', '!', '*', '0', '#', 'R'};


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

long bpm = 90.0;
long bpmMin=20.0;
long bpmMax=800.0;
long tempo = 1000.0/(bpm/60.0); //bpm in milliseconds

float prevPulseStart = 0; //previous millisecond timestamp before last pulse was started
float prevClockStart = 0; //previous millisecond timestamp before last clock signal was sent
float clockInterval = tempo/24; //number of milliseconds after which to send the midi clock. MIDI spec is to send clock 24 times per quarter note
float pulseStart = 0; //timestamp when the pulse started
float noteStart = 0; //timestamp when the note started



bool run = false;
bool reset = false;
int numSteps = 8; // how many steps should be done. Jumps back to first step after numSteps
const int maxSteps = 8; // Maximum number of Steps of your sequencer
int seqDirection = 1; // Sequencer step direction. 1=up, 0=down

int pulsePointer = 0; //points to the pulse within the step we're currently in
int lastStepPointer = 0; //previous step, to trigger note off
int lastNoteSent = 0; //previous note sent, to trigger note off on time
int pulseCount [] {1, 1, 1, 1, 1, 1, 1, 1}; // how many times a step should be played
int gateMode [maxSteps] {2, 2, 2, 2, 2, 2, 2, 2}; //0=no gate, 1=first gate, 2=every gate
bool skipStep [maxSteps] {0, 0, 0, 0, 0, 0, 0, 0}; //1=skip a step, 0=don't skip
int playMode = 0; // Sequencer play order. 0=forwards, 1=backwards, 2=ping-pong,3=drunk
const int numPlayModes = 4; // how many play modes there are of the sequencer
bool fuSel0 = 0; // Sequencer button Function Selectors. Act as 2bit input field to select
bool fuSel1 = 0; // what the sequencer buttons do. 
int seqButtonFunction = 0; //Int representation of the selected function. 0=Play, 1=Skip, 2=Slide, 3=hold

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
  {0, 0, 0, 0, 0, 0, 0, 0},   // Step Value (Note)
  {0, 0, 0, 0, 0, 0, 0, 0},   // Skip Step
  {0, 0, 0, 0, 0, 0, 0, 0},   // Slide Step
  {0, 0, 0, 0, 0, 0, 0, 0}    // Hold
};

bool noteStopped = true; //if note has been stopped for this step already

int stepPointer = 0; //pointer, points to current step that we're at

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

// stores if value was changed during this iteration
bool arr_changed_analog_inputs [numAnalogInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 0, 0}, //I1
  {0, 0, 0, 0, 0, 0, 0, 0}, //I2
  {0, 0, 0, 0, 0, 0, 0, 0}, //I3
  {0, 0, 0, 0, 0, 0, 0, 0}, //I5
  {0, 0, 0, 0, 0, 0, 0, 0} //I6
};

bool arr_changed_digital_inputs [numDigitalInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 0, 0}, //I4
  {0, 0, 0, 0, 0, 0, 0, 0} //I8
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

// if a pin is "slowread", should not be read every cycle.
// how many cycles to skip is defined in digitalSmoother
bool arr_slow_digital_inputs [numDigitalInMux][pinPerMux] {
  {1, 1, 1, 1, 1, 1, 1, 1}, //I4
  {0, 0, 0, 0, 0, 0, 0, 0} //I8
};



// stores the type of input. 0=MIDI CC, 1=internal, value is only used on teensy/arduino and not sent as MIDI CC
bool arr_analog_input_type [numAnalogInMux][pinPerMux] {
  {0, 0, 0, 0, 0, 0, 0, 0}, //I1
  {0, 0, 0, 0, 0, 0, 0, 0}, //I2
  {1, 1, 1, 1, 1, 1, 1, 1}, //I3
  {1, 1, 1, 1, 1, 1, 1, 1}, //I5
  {1, 1, 1, 1, 1, 1, 1, 1} //I6
};

bool arr_digital_input_type [numDigitalInMux][pinPerMux] {
  {1, 1, 1, 1, 1, 1, 1, 1}, //I4
  {1, 1, 1, 1, 1, 1, 1, 1} //I8
};

///
/// Functions
///

//Function Prototypes
void nextDrumStep(void);
int youFuckenKiddingMe(char);
void startNote(int);
void stopNote(int);
void selectSeqNoteFunction(void);
void noteButtonPressed(int note);
void noteButtonReleased(int note);
void updateDrumTempoModifier(void);

// what happens when an external clock signal is received
void handleClock() {

  //Drumstep: only when drum running and NOT synced to sequencer
  if (runDrum) {
    if (!syncDrumToSequencer) {
      nextDrumStep();
    }
  }

  //SEQ step: only when synced to external clock
  if (syncSequencerToClock) {
    nextPulse();
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
  //delay(1); //maybe add back if you got stability issues, however it causes latency
}

// selects the pin on output multiplexer
void selectMuxOutPin(byte pin){
  bool valA = bitRead(pin, 0);
  bool valB = bitRead(pin, 1);
  bool valC = bitRead(pin, 2);

  digitalWrite(Aout, valA);
  digitalWrite(Bout, valB);
  digitalWrite(Cout, valC);
  //delay(1); //maybe add back if you got stability issues, however it causes latency
}

void readAnalogPins() {
  // loop through analog in multiplexers
  for (int mux = 0; mux<numAnalogInMux; mux++){
    
    //loop through every pin for the analog multiplexers
    for (byte pin=0; pin<pinPerMux; pin++){
        arr_changed_analog_inputs[mux][pin] = 0;
        selectMuxInPin(pin);
        // only read if input is not disabled
        if (!arr_disable_analog_inputs[mux][pin]){
          rawAnalog = analogRead(arr_pin_analog_inputs[mux]);
          rawAnalog = map(rawAnalog,0,1023,0,127);
          //calculate difference of the read values. square and squareroot to always get positive difference
          // Serial.print("/");
          // Serial.print(analogReadDiff);
          analogReadDiff = arr_prev_read_analog_inputs[mux][pin] - rawAnalog;
          // Serial.print("/");
          // Serial.print(analogReadDiff);
          analogReadDiff = sq(analogReadDiff);
          // Serial.print("/");
          // Serial.print(analogReadDiff);
          analogReadDiff = sqrt(analogReadDiff);
          // Serial.print("/");
          // Serial.println(analogReadDiff);
          if (analogReadDiff > analogSmoother) {
            arr_changed_analog_inputs[mux][pin] = 1;
            arr_prev_read_analog_inputs[mux][pin] = arr_read_analog_inputs[mux][pin];
            arr_read_analog_inputs[mux][pin] = rawAnalog;
          }
        }
        
        
      }
  }
}

void readDigitalPins() {
  // loop through digital in multiplexers
  for (int mux = 0; mux<numDigitalInMux; mux++){
    
    //loop through every pin for the digital muxes
    for (byte pin=0; pin<pinPerMux; pin++){
        arr_changed_digital_inputs[mux][pin] = 0;
        selectMuxInPin(pin);
        delay(1);
        // only read if input is not disabled
        if (!arr_disable_digital_inputs[mux][pin]){
          rawDigital = !digitalRead(arr_pin_digital_inputs[mux]); //! because inverted logic
          // only update digital if not same as previous value and if not same as current reading
          if (arr_read_digital_inputs[mux][pin] != rawDigital){
            // don't read if its a slow button, except in slowReadCycles
            if (arr_slow_digital_inputs[mux][pin]) {
              if (!isSlowReadCycle) {
                break;
              }
            }
            arr_changed_digital_inputs[mux][pin] = 1;
            arr_prev_read_digital_inputs[mux][pin] = arr_read_digital_inputs[mux][pin];
            arr_read_digital_inputs[mux][pin] = rawDigital;
          }
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
      
      // only act if value changed
      //if (arr_prev_read_analog_inputs[mux][pin] != arr_read_analog_inputs[mux][pin]) {
      if (arr_changed_analog_inputs[mux][pin]) {
        //Serial.println("mux ");
        // Serial.print(mux);
        // Serial.print(" pin ");
        // Serial.print(pin);
        // Serial.print("Controller ");
        // Serial.print(arr_send_analog_inputs[mux][pin]);
        // Serial.print(": ");
        // Serial.println(arr_read_analog_inputs[mux][pin]);

        // Type = MIDI CC
        if (!arr_analog_input_type[mux][pin]) {
          usbMIDI.sendControlChange(arr_send_analog_inputs[mux][pin], arr_read_analog_inputs[mux][pin], synthMidiChannel);
        } else {
          //Type = Internal Variable
          //Mapping of Controller Number (from arr_send_analog_inputs) to internal variables
          switch (arr_send_analog_inputs[mux][pin]){
            // case 1:

            // break;
            // case 2:

            // break;
            // case 3:

            // break;
            // case 4:

            // break;
            // case 5:

            // break;
            // case 6:

            // break;
            // case 7:

            // break;
            // case 8:

            // break;
            // case 9:

            // break;
            // case 10:

            // break;
            // case 11:

            // break;
            // case 12:

            // break;
            // case 13:

            // break;
            // case 14:

            // break;
            // case 15:

            // break;
            // case 16:

            // break;
            case 17:
              //Clock / BPM
              if (!syncSequencerToClock){
                bpm = map(arr_read_analog_inputs[2][0],0,127,bpmMin,bpmMax);
              }
              // if (syncDrumToSequencer) {
              //   updateDrumTempoModifier(map(arr_read_analog_inputs[2][0],0,127,0,13));
              // }
            break;
            case 18:
              // number of steps the sequencer should play
              numSteps = map(arr_read_analog_inputs[2][1],0,127,1,maxSteps);
            break;
            case 19:
              //Gate length
              gateTime = map(arr_read_analog_inputs[2][2],0,127,gateMin,gateMax);
            break;
            case 20:
              //Slide Amount
              slideAmount = arr_read_analog_inputs[2][3];
            break;
            // case 21:
            // break;
            // case 22:

            // break;
            // case 23:

            // break;
            // case 24:

            // break;

            // Seq Note Faders
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
            case 32:
              //-25 so that 25 is 0, 26 is 1 etc...
              caseNumber = arr_send_analog_inputs[mux][pin];
              noteNumberAnalog = caseNumber-25;
              arr_seq_buttons[0][noteNumberAnalog] = map(arr_read_analog_inputs[3][noteNumberAnalog],0,127,minSeqNote,maxSeqNote);
            break;

            // seq Pulse count, how many pulses per step
            case 33:
            case 34:
            case 35:
            case 36:
            case 37:
            case 38:
            case 39:
            case 40:
              //-33 so that 33 is 0, 34 is 1 etc...
              caseNumber = arr_send_analog_inputs[mux][pin];
              noteNumberAnalog = caseNumber-33;
              pulseCount[noteNumberAnalog] = map(arr_read_analog_inputs[4][noteNumberAnalog],0,127,minPulse,maxPulse);
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
      //if (arr_prev_read_digital_inputs[mux][pin] != arr_read_digital_inputs[mux][pin]) {
      if (arr_changed_digital_inputs[mux][pin]) {
        // Serial.print("Digital ");
        // Serial.print(arr_send_digital_inputs[mux][pin]);
        // Serial.print(": ");
        // Serial.println(arr_read_digital_inputs[mux][pin]);


        // Type = MIDI CC
        if (!arr_digital_input_type[mux][pin]) {
          //usbMIDI.sendControlChange(arr_send_digital_inputs[mux][pin], arr_read_digital_inputs[mux][pin], synthMidiChannel);
        } else {
          //Type = Internal Variable
          //Mapping of Controller Number (from arr_send_digital_inputs) to internal variables
          switch (arr_send_digital_inputs[mux][pin]){
              case 41:
                if (!syncSequencerToClock){
                  //Sequencer Play/pause
                  if (arr_read_digital_inputs[0][0] && arr_changed_digital_inputs[0][0]) {
                    run = !run;
                    if (syncDrumToSequencer){
                      runDrum = !runDrum;
                    }
                    if (run){
                      // Serial.println("play");
                      prevClockStart = millis();
                      prevPulseStart = millis();
                    } else {
                      // Serial.println("pause");
                      stopLastNote();
                      stopNote(stepPointer);
                    }
                  }
                }
                
              break;
              case 42:
                //Sequencer Reset
                if (arr_read_digital_inputs[0][1] && arr_changed_digital_inputs[0][1]) {
                  reset = arr_read_digital_inputs[0][1];
                }
              break;
              case 43:
              if (blnTapTempo){
                  tapTempo.update(!arr_read_digital_inputs[0][2]);
                  bpm = long(tapTempo.getBPM());
                  Serial.println(bpm);
                }
              if (arr_read_digital_inputs[0][2] && arr_changed_digital_inputs[0][2]){
                if (!blnTapTempo){
                    //Sequencer Play Mode
                    playMode++;
                    if (playMode>=numPlayModes){
                      playMode = 0;
                  }
                }
              }
                
              break;
              case 44:
                //Clear selected SeqButton function
                if (arr_read_digital_inputs[0][3] && arr_changed_digital_inputs[0][3]){
                  //except for 0=playMode
                  if (seqButtonFunction!=0){
                    for (int i = 0; i<pinPerMux; i++)
                    {
                      arr_seq_buttons[seqButtonFunction][i]=0;
                    }
                  }
                }
                
              break;
              case 45:
                //SeqButton Function Selector0 (fuSel0)
                if (arr_changed_digital_inputs[0][4]){
                  fuSel0 = arr_read_digital_inputs[0][4];
                  selectSeqNoteFunction();
                }
              break;
              case 46:
                //SeqButton Function Selector1 (fuSel1)
                if (arr_changed_digital_inputs[0][5]){
                  fuSel1 = arr_read_digital_inputs[0][5];
                  selectSeqNoteFunction();
                }
              break;
              case 47:
                //disabled
              break;
              case 48:
                //disabled
              break;

              //notebuttons
              case 49:
              case 50:
              case 51:
              case 52:
              case 53:
              case 54:
              case 55:
              case 56:
                caseNumber = arr_send_digital_inputs[mux][pin];
                //-49, so 49 will be 0, 50 will be 1 etc...
                noteNumberDigiRead = caseNumber-49;

                switch (seqButtonFunction) {
                  case 0:
                    //only play notes when sequencer is not running, and not synced to ext clock
                    if (!run && !syncSequencerToClock){
                     if (arr_read_digital_inputs[1][noteNumberDigiRead]){
                        noteButtonPressed(noteNumberDigiRead);
                      } else {
                        noteButtonReleased(noteNumberDigiRead);
                      } 
                    }
                    
                  break;
                  case 1: //skip step
                    if (arr_read_digital_inputs[1][noteNumberDigiRead] && arr_changed_digital_inputs[1][noteNumberDigiRead]){
                      arr_seq_buttons[1][noteNumberDigiRead] = !arr_seq_buttons[1][noteNumberDigiRead];
                    }
                  break;
                  case 2: //slide step
                    if (arr_read_digital_inputs[1][noteNumberDigiRead] && arr_changed_digital_inputs[1][noteNumberDigiRead]){
                      arr_seq_buttons[2][noteNumberDigiRead] = !arr_seq_buttons[2][noteNumberDigiRead];
                    }
                  break;
                  case 3: //hold step
                    if (arr_read_digital_inputs[1][noteNumberDigiRead] && arr_changed_digital_inputs[1][noteNumberDigiRead]){
                      arr_seq_buttons[3][noteNumberDigiRead] = !arr_seq_buttons[3][noteNumberDigiRead];
                    }
                    
                    
                    break;
                  }
              break;
  //5 -> typo, but a sweet one :) stay a dreamer!

          }
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
  pinMode(I7, OUTPUT);
  pinMode(I8, INPUT_PULLUP);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(Aout, OUTPUT);
  pinMode(Bout, OUTPUT);
  pinMode(Cout, OUTPUT);


  usbMIDI.setHandleClock(handleClock);
  selectMuxOutPin(stepPointer);
  digitalWrite(I7, HIGH);


}

void loop() {
  startLoopMillis = millis();
  slowReadCycleCount++;
  if (slowReadCycleCount > digitalSmoother) {
    isSlowReadCycle = 1;
    slowReadCycleCount = 0;
  }

  // save at what millisecond the loop starts, for timing
  currentMillis = millis();

  //reading drumpad after every function for lower latency
  readDrumpad();
  drumPadMillis = millis();
  readAnalogPins();
  analogReadMillis = millis();
  readDrumpad();
  readDigitalPins();
  digitalReadMillis = millis();
  readDrumpad();
  usbMIDI.read();

  UpdateSendValues();
  updateValueMillis = millis();
  updateTempo();

  if (reset){
    resetSequencer();
    currentMillis = millis();
  }

  //sequencer step trigger
  if (run) {
    // send clock if necessary
    if (currentMillis - prevClockStart >= clockInterval) {
      //save timestamp when pulse started
      prevClockStart = currentMillis;
      usbMIDI.sendClock();
    }
    seqNotesMillis = millis();
    // do next pulse if necessary
    
    if (!syncSequencerToClock) {
      if (currentMillis - prevPulseStart >= tempo) {
        //save timestamp when pulse started
        prevPulseStart = currentMillis;
        nextPulse(); //after last pulse of a step, next step will be triggered
        if (syncDrumToSequencer){
          if (runDrum){
            nextDrumStep();
          }
    }
      }
    }
    
    
    //stop note if necessary
    if (!noteStopped) {
      if (currentMillis - noteStart >= gateTime) {
        stopLastNote();
      }
    }
    
    if (!drumNoteStopped) {
      if (currentMillis - drumNoteStart >= drumGateTime) {
        stopAllDrumNotes();
      }
    }
  }

  isSlowReadCycle = 0; //
  //benchmarking
//   endLoopMillis = millis();
//   Serial.print(drumPadMillis - startLoopMillis);
//   Serial.print("|");
//   Serial.print(analogReadMillis  - startLoopMillis);
//   Serial.print("|");
//   Serial.print(digitalReadMillis - startLoopMillis);
//   Serial.print("|");
//   Serial.print(updateValueMillis - startLoopMillis);
//   Serial.print("|");
//   Serial.print(seqNotesMillis - startLoopMillis);
//   Serial.print("|");
//   Serial.println(endLoopMillis - startLoopMillis);
}
