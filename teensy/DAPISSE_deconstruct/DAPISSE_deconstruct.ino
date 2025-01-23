#include <Keypad.h>
#include <ArduinoTapTempo.h>
#include <math.h>
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


//sequencer
//const int note = 42;

//bool syncSequencerToClock = 0; // 1 = next sequencer note triggered on ext clock signal | 0 = internal bpm used as step tempo
//bool syncDrumToSequencer = 1; //1 = drum step triggered when sequencer steps | 0 = external clock received
//bool drumDivMultMode = false; // true = trigger drum steps, when "fill each n step" is activated
bool syncClockToExt = false; //trigger next clock cycle if midi Clock is received (untested)
int synthMidiChannel = 2;
bool blnTapTempo = true; //1 = playMode button becomes tapTempo button 
///
/// Variable definitions
///
ArduinoTapTempo tapTempo;

int analogReadDiff = 0;
int caseNumber = 0;
int noteNumberDigiRead = 0;
int noteNumberAnalog = 0;
float currentMillis = 0;

//for benchmark purposes
int startLoopMillis = 0;
int drumPadMillis = 0;
int analogReadMillis = 0;
int digitalReadMillis = 0;
int updateValueMillis = 0;
int seqNotesMillis = 0;
int endLoopMillis = 0;

// ############
// Clock
// ############

//bpm








// ############
// DRUMS
// ############
// number of instruments (0=kick, 1=snare, 2=highhat, 3=cymbal)
const int numDrumInstruments = 4;
const int numDrumPatterns = 4; //drum patterns. preset beats to be selected from
int midiC = 60; // Standard midi C
int transpose = 0;
//float tempoModifier[numDrumInstruments] = {1, 1, 1, 1}; //multiplication value with sequencer tempo to get n-times the tempo of the clock
//bool tempoOperation[numDrumInstruments] = {false, false, false, false}; // False = division (fill each n step), true = multiplication (n notes per step)
// hehe, it says poop


// which midi note to play for each instrument
int instrumentNotes[2][numDrumInstruments] = {
  {3, 7, 11, 15}, //keynumber
  {63, 67, 71, 75}, //midinote
  //{0, 0, 0, 0} // tempo in ms (1000 = 60bpm) set to 0ms to disable
};
//60=C, 64 = E, 63 = D#, 67 = G

// which instrument is currently selected for keypad play mode

int selectedInstrument = 0;
int selectedDrumPattern = 1; //which drum pattern is selected
int drumMidiChannel = 3;
//bool drumNoteStopped[numDrumInstruments] = {true, true, true, true}; //if drum notes have been stopped for this step already (goes for all drum notes)
//float drumNoteStart[numDrumInstruments] = {0, 0, 0, 0}; //timestamp when the drum note started
//float prevDrumNoteStart[numDrumInstruments] = {0, 0, 0, 0}; //timestamp when last drum note started
//int drumGateTime = 2; //time in ms how long the drum note should be on
bool holdFired = false; //set to true if a button was holded will suppress release event trigger

//number of notes / steps the drum sequencer has
const int numDrumSteps = 96; // use whatever subtick was defined and multiply by 4 for 4 beats
// 24*4 = 96

// pointer at which step in the drum sequencer we are
int drumStepPointer = 0;

int keypadMode = 2; //0 = play and fill each x step
                    //1 = enable/disable by keynumber
                    //2 = play and record
                    //experiment: just use play and add recordDrum switch
                    //originally: 0=play, 1=sequence notes 2=fillXStep, 4=settings
bool runDrum = true;
bool recordDrum = true; // true = played notes are recorded into the drumSequence
//bools if a drum sound should be triggered at the selected step
bool drumSequence [numDrumPatterns][numDrumInstruments][numDrumSteps] {
  { //rock
    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 1, 0, 0, 0, 0}, //kick

    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 1, 0, 0, 0, 0}, //snare

    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 1, 0, 0, 0, 0}, //hat

    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 1, 0, 0, 0, 0}, //cymbal
  },
  { //empty / test / DIY
    {0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 1, 0, 0, 0, 0}, //kick

    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 1, 0, 0, 0, 0}, //snare

    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 1, 0, 0, 0, 0}, //hat

    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    0, 1, 0, 0, 0, 0}, //cymbal
  },
  { //dnb
    {0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0}, //kick

    {0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0}, //snare

    {0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0}, //hat

    {0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0}, //cymbal
  },
  { //ska
    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 1, 0, 0, 0, 0}, //kick

    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 1, 0, 0, 0, 0}, //snare

    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 1, 0, 0, 0, 0}, //hat

    {1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,     1, 0, 0, 0, 0, 0,    1, 0, 0, 0, 0, 0,    1, 1, 0, 0, 0, 0}, //cymbal
  },
};



//float prevPulseStart = 0; //previous millisecond timestamp before last pulse was started
//float pulseStart = 0; //timestamp when the pulse started
//float noteStart = 0; //timestamp when the note started
//bool noteStopped = true; //if note has been stopped for this step already


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
int youFuckenKiddingMeToo(char);
void startNote(int);
void stopNote(int);
void selectSeqNoteFunction(void);
void noteButtonPressed(int);
void noteButtonReleased(int);
void updateDrumModifier(int);
void changeDrumPattern(bool);
void recordKey(int, int);
int getDrumNote(int);
int getDrumIndex(int);
int getCurrentTick(void);
void resetClock(void);
int getSubClockIndexByInstrument(int);

// what happens when an external clock signal is received
void handleClock() {

  if (syncClockToExt) {
    if (run){
      nextPulse();
    }
  }

  ///OBSOLETE
  // //Drumstep: only when drum running and NOT synced to sequencer
  // if (runDrum) {
  //   if (!syncDrumToSequencer) {
  //     nextDrumStep();
  //   }
  // }

  // //SEQ step: only when synced to external clock
  // if (syncSequencerToClock) {
    
  // }

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
              //if (!syncSequencerToClock){
              bpm = map(arr_read_analog_inputs[2][0],0,127,bpmMin,bpmMax);
              //}
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
                //Sequencer Play/pause
                if (arr_read_digital_inputs[0][0] && arr_changed_digital_inputs[0][0]) {
                  run = !run;
                  if (run){
                    // Serial.println("play");
                    //prevClockStart = millis();
                    //prevPulseStart = millis();
                    resetClock();
                  } else {
                    // Serial.println("pause");
                    stopLastNote();
                    stopNote(stepPointer);
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

  for (int i = 0; i < numSubClocks; i++) {
    subClocks[i][13] = subClocks[i][4];  // Store set delay to delay buffer
  }
}

void loop() {
  startLoopMillis = millis();
  slowReadCycleCount++;
  if (slowReadCycleCount > digitalSmoother) {
    isSlowReadCycle = 1;
    slowReadCycleCount = 0;
  }
  //reading drumpad and updating clock after every function for lower latency
  readDrumpad();
  updateClock();
  
  drumPadMillis = millis();
  readAnalogPins();
  analogReadMillis = millis();

  readDrumpad();
  updateClock();
  readDigitalPins();
  digitalReadMillis = millis();
  
  readDrumpad();
  updateClock();
  usbMIDI.read();

  UpdateSendValues();
  updateValueMillis = millis();

  if (reset){
    resetSequencer();
    resetClock();
  }

  updateClock();
  isSlowReadCycle = 0; //
  
  if (initialClockReset && currentMillis > initialClockResetTime){
    initialClockReset = false;
    resetClock();
  }

  ///OBSOLETE due to new clock
  //sequencer step trigger
  //if (run) {
  //  // send clock if necessary
  //   if (currentMillis - prevClockStart >= tickMS) {
  //     //save timestamp when pulse started
  //     prevClockStart = currentMillis;
  //     usbMIDI.sendClock();
  //   }
  //   seqNotesMillis = millis();
  //   // do next pulse if necessary
  //   if (!syncSequencerToClock) {
  //     if (currentMillis - prevPulseStart >= tempo) {
  //       //save timestamp when pulse started
  //       prevPulseStart = currentMillis;
  //       nextPulse(); //after last pulse of a step, next step will be triggered
  //       if (syncDrumToSequencer && runDrum && !drumDivMultMode){
  //           nextDrumStep();
  //       }
  //     }
  //   }

  //   if (drumDivMultMode && runDrum){
  //       for (int i = 0; i < numDrumInstruments; i++) {

  //         if (currentMillis - prevDrumNoteStart[i] >= instrumentNotes[2][i] && instrumentNotes[2][i] != 0){ // note time set to 0ms means disabled
  //           prevDrumNoteStart[i] = currentMillis;
  //           startDrumNote(instrumentNotes[1][i], i);
  //         }
  //       }
  //   }
    
    
  //stop notes if necessary (in case clock is not running)
  for (int i = 0; i < numSubClocks; i++){
    if (subClocks[i][11] > (currentMillis + subClocks[i][7])) { //if gate time is over
      if (subClocks[i][10] == 0) { //stop not sent
        if (subClocks[i][6] > 10){ //sequencer instrument
          stopLastNote(); 
        }
        else // drum instrument
        {
          stopDrumNote(instrumentNotes[1][int(subClocks[i][6])],(i-1)); //hacky, i-1 only currently is correct
        }
        
      }
    }
  }
  // }

  //benchmarking
  // endLoopMillis = millis();
  // Serial.print(drumPadMillis - startLoopMillis);
  // Serial.print("|");
  // Serial.print(analogReadMillis  - startLoopMillis);
  // Serial.print("|");
  // Serial.print(digitalReadMillis - startLoopMillis);
  // Serial.print("|");
  // Serial.print(updateValueMillis - startLoopMillis);
  // Serial.print("|");
  // Serial.print(seqNotesMillis - startLoopMillis);
  // Serial.print("|");
  // Serial.println(endLoopMillis - startLoopMillis);


  //28.08.2024: 0|1|17|17|-95237|17
}
