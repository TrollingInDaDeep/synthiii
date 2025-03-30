#include <Control_Surface.h>

// Instantiate a MIDI Interface to use
USBMIDI_Interface midi;
const  int synthMidiChannel = 12;

//din1
//I7

// PINS
const int I1 = 38;    //Analog In 2 -> synth rechts
const int I2 = 39;    //Analog In 2 -> synth links
const int I3 = 41;    //Analog In 2 -> seq links pot
const int I4 = 17;    //Analog In 3 -> seq links btn
const int I5 = 15;    //Analog In 3 -> seq faders
const int I6 = 16;    //Analog In 3 -> seq pulse
const int I7 = 32;    //Digital Out -> seq LEDs //WARNING: ADD RESISTOR BEFORE LEDS
const int I8 = 14;    //Analog In 3 -> seq btn triggers
const int I9 = 24;    //Analog In 1 -> topPlate pots 1
const int I10 = 25;    //Analog In 1 -> topPlate pots 2
const int I11 = 26;    //Analog In 1-> topPlate pots 3
const int I12 = 27;    //Analog In 1-> topPlate pots 4
const int I13 = 40;   //Analog In 2 -> seq Gatemode
const int A = 33;     //Digital Out
const int B = 34;     //Digital Out
const int C = 35;     //Digital Out
const int Aout = 36;  //Digital Out
const int Bout = 37;  //Digital Out
const int Cout = 23;  //Digital Out

//Multiplexers
CD74HC4051 muxI1 {I1, {A, B, C} };
CD74HC4051 muxI2 {I2, {A, B, C} };
CD74HC4051 muxI3 {I3, {A, B, C} };
CD74HC4051 muxI4 {I4, {A, B, C} };
CD74HC4051 muxI5 {I5, {A, B, C} };
CD74HC4051 muxI6 {I6, {A, B, C} };
//CD74HC4051 muxI7 {I7, {Aout, Bout, Cout} };
CD74HC4051 muxI8 {I8, {A, B, C} };
CD74HC4051 muxI9 {I9, {A, B, C} };
CD74HC4051 muxI10 {I10, {A, B, C} };
CD74HC4051 muxI11 {I11, {A, B, C} };
CD74HC4051 muxI12 {I12, {A, B, C} };
CD74HC4051 muxI13 {I13, {A, B, C} };

// Controls
//CCPotentiometer
CCPotentiometer I1_POTS[] {
    { muxI1.pin(0), {0x00, Channel::createChannel(synthMidiChannel)} },
    { muxI1.pin(1), {0x01, Channel::createChannel(synthMidiChannel)} },
    { muxI1.pin(2), {0x02, Channel::createChannel(synthMidiChannel)} },
    { muxI1.pin(3), {0x03, Channel::createChannel(synthMidiChannel)} },
    { muxI1.pin(4), {0x04, Channel::createChannel(synthMidiChannel)} },
    { muxI1.pin(5), {0x05, Channel::createChannel(synthMidiChannel)} },
    //{ muxI1.pin(6), {0x06, Channel::createChannel(synthMidiChannel)} }, //not Connected
    //{ muxI1.pin(7), {0x07, Channel::createChannel(synthMidiChannel)} }
};
CCPotentiometer I2_POTS[] {
    { muxI2.pin(7), {0x08, Channel::createChannel(synthMidiChannel)} },
    { muxI2.pin(6), {0x09, Channel::createChannel(synthMidiChannel)} },
    { muxI2.pin(5), {0x0A, Channel::createChannel(synthMidiChannel)} },
    { muxI2.pin(4), {0x0B, Channel::createChannel(synthMidiChannel)} },
    { muxI2.pin(3), {0x0C, Channel::createChannel(synthMidiChannel)} },
    { muxI2.pin(2), {0x0D, Channel::createChannel(synthMidiChannel)} },
    { muxI2.pin(1), {0x0E, Channel::createChannel(synthMidiChannel)} },
    { muxI2.pin(0), {0x0F, Channel::createChannel(synthMidiChannel)} }
};

CCPotentiometer I3_POTS[] {
    //{ muxI3.pin(0), {0x10, Channel::createChannel(synthMidiChannel)} },
    //{ muxI3.pin(1), {0x11, Channel::createChannel(synthMidiChannel)} },//unused

    //{ muxI3.pin(2), {0x12, Channel::createChannel(synthMidiChannel)} },
    // { muxI3.pin(3), {0x13, Channel::createChannel(synthMidiChannel)} }, //used internally
    // { muxI3.pin(4), {0x14, Channel::createChannel(synthMidiChannel)} }, 
    // { muxI3.pin(5), {0x15, Channel::createChannel(synthMidiChannel)} }, //slide amount
    //{ muxI3.pin(6), {0x16, Channel::createChannel(synthMidiChannel)} }, //minNote
    //{ muxI3.pin(7), {0x17, Channel::createChannel(synthMidiChannel)} },  //maxNote
};
CCButton I4_BUTTONS[] {
    // { muxI4.pin(0), {0x18, Channel::createChannel(synthMidiChannel)} },
    // { muxI4.pin(1), {0x19, Channel::createChannel(synthMidiChannel)} },
    // { muxI4.pin(2), {0x1A, Channel::createChannel(synthMidiChannel)} },
    // { muxI4.pin(3), {0x1B, Channel::createChannel(synthMidiChannel)} }, //all used internally
    // { muxI4.pin(4), {0x1C, Channel::createChannel(synthMidiChannel)} },
    // { muxI4.pin(5), {0x1D, Channel::createChannel(synthMidiChannel)} },
    { muxI4.pin(6), {0x1E, Channel::createChannel(synthMidiChannel)} } //sync switch for synth 
    //{ muxI4.pin(7), {0x1F, Channel::createChannel(synthMidiChannel)} } //not connected
};

//used internally
// CCPotentiometer I5_POTS[] {
//     { muxI5.pin(7), {0x20, Channel::createChannel(synthMidiChannel)} },
//     { muxI5.pin(6), {0x21, Channel::createChannel(synthMidiChannel)} },
//     { muxI5.pin(5), {0x22, Channel::createChannel(synthMidiChannel)} },
//     { muxI5.pin(4), {0x23, Channel::createChannel(synthMidiChannel)} },
//     { muxI5.pin(3), {0x24, Channel::createChannel(synthMidiChannel)} },
//     { muxI5.pin(2), {0x25, Channel::createChannel(synthMidiChannel)} }, // not working, buggy
//     { muxI5.pin(1), {0x26, Channel::createChannel(synthMidiChannel)} },
//     { muxI5.pin(0), {0x27, Channel::createChannel(synthMidiChannel)} },
// };

//used internally
// CCPotentiometer I6_POTS[] {
//     { muxI6.pin(0), {0x28, Channel::createChannel(synthMidiChannel)} },
//     { muxI6.pin(1), {0x29, Channel::createChannel(synthMidiChannel)} },
//     { muxI6.pin(2), {0x2A, Channel::createChannel(synthMidiChannel)} },
//     { muxI6.pin(3), {0x2B, Channel::createChannel(synthMidiChannel)} },
//     { muxI6.pin(4), {0x2C, Channel::createChannel(synthMidiChannel)} },
//     { muxI6.pin(5), {0x2D, Channel::createChannel(synthMidiChannel)} },
//     { muxI6.pin(6), {0x2E, Channel::createChannel(synthMidiChannel)} },
//     { muxI6.pin(7), {0x2F, Channel::createChannel(synthMidiChannel)} }
// };
// CCPotentiometer I7_POTS[] {
//     { muxI7.pin(0), {0x30, Channel::createChannel(synthMidiChannel)} },
//     { muxI7.pin(1), {0x31, Channel::createChannel(synthMidiChannel)} },
//     { muxI7.pin(2), {0x32, Channel::createChannel(synthMidiChannel)} },
//     { muxI7.pin(3), {0x33, Channel::createChannel(synthMidiChannel)} },
//     { muxI7.pin(4), {0x34, Channel::createChannel(synthMidiChannel)} },
//     { muxI7.pin(5), {0x35, Channel::createChannel(synthMidiChannel)} },
//     { muxI7.pin(6), {0x36, Channel::createChannel(synthMidiChannel)} },
//     { muxI7.pin(7), {0x37, Channel::createChannel(synthMidiChannel)} },
// };

//NoteButton functionality if enabled for seq trigger buttons
// initialize with default value
//would be great but notebuttons only work with a fixed value

//on top of that, why the heck ever, the seqButton[] buttons ONLY work
// if one pin on that multiplexer is also initialized as a midi button
CCButton SEQ_NoteButtons[] {
    //{ muxI8.pin(7), {MIDI_Notes::C[4], Channel::createChannel(synthMidiChannel)} },
    //{ muxI8.pin(6), {MIDI_Notes::C[4], Channel::createChannel(synthMidiChannel)} },
    //{ muxI8.pin(5), {MIDI_Notes::C[4], Channel::createChannel(synthMidiChannel)} },
    //{ muxI8.pin(4), {MIDI_Notes::C[4], Channel::createChannel(synthMidiChannel)} },
    //{ muxI8.pin(3), {MIDI_Notes::C[4], Channel::createChannel(synthMidiChannel)} },
    //{ muxI8.pin(2), {MIDI_Notes::C[4], Channel::createChannel(synthMidiChannel)} },
    //{ muxI8.pin(1), {MIDI_Notes::C[4], Channel::createChannel(synthMidiChannel)} },
    { muxI8.pin(0), {127, Channel::createChannel(synthMidiChannel)} },
};

CCPotentiometer I9_POTS[] {
    { muxI9.pin(0), {0x40, Channel::createChannel(synthMidiChannel)} },
    { muxI9.pin(1), {0x41, Channel::createChannel(synthMidiChannel)} },
    { muxI9.pin(2), {0x42, Channel::createChannel(synthMidiChannel)} },
    { muxI9.pin(3), {0x43, Channel::createChannel(synthMidiChannel)} },
    { muxI9.pin(4), {0x44, Channel::createChannel(synthMidiChannel)} },
    { muxI9.pin(5), {0x45, Channel::createChannel(synthMidiChannel)} },
    { muxI9.pin(6), {0x46, Channel::createChannel(synthMidiChannel)} },
    { muxI9.pin(7), {0x47, Channel::createChannel(synthMidiChannel)} },
};


CCPotentiometer I10_POTS[] {
    { muxI10.pin(0), {0x48, Channel::createChannel(synthMidiChannel)} },
    { muxI10.pin(1), {0x49, Channel::createChannel(synthMidiChannel)} },
    { muxI10.pin(2), {0x4A, Channel::createChannel(synthMidiChannel)} },
    { muxI10.pin(3), {0x4B, Channel::createChannel(synthMidiChannel)} },
    { muxI10.pin(4), {0x4C, Channel::createChannel(synthMidiChannel)} },
    { muxI10.pin(5), {0x4D, Channel::createChannel(synthMidiChannel)} },
    { muxI10.pin(6), {0x4E, Channel::createChannel(synthMidiChannel)} },
    { muxI10.pin(7), {0x4F, Channel::createChannel(synthMidiChannel)} },
};

CCPotentiometer I11_POTS[] {
    { muxI11.pin(0), {0x50, Channel::createChannel(synthMidiChannel)} },
    { muxI11.pin(1), {0x51, Channel::createChannel(synthMidiChannel)} },
    { muxI11.pin(2), {0x52, Channel::createChannel(synthMidiChannel)} },
    { muxI11.pin(3), {0x53, Channel::createChannel(synthMidiChannel)} },
    { muxI11.pin(4), {0x54, Channel::createChannel(synthMidiChannel)} },
    { muxI11.pin(5), {0x55, Channel::createChannel(synthMidiChannel)} },
    { muxI11.pin(6), {0x56, Channel::createChannel(synthMidiChannel)} },
    { muxI11.pin(7), {0x57, Channel::createChannel(synthMidiChannel)} },
};

CCPotentiometer I12_POTS[] {
    { muxI12.pin(0), {0x58, Channel::createChannel(synthMidiChannel)} },
    { muxI12.pin(1), {0x59, Channel::createChannel(synthMidiChannel)} },
    //{ muxI12.pin(2), {0x5A, Channel::createChannel(synthMidiChannel)} }, //used internally
    //{ muxI12.pin(3), {0x5B, Channel::createChannel(synthMidiChannel)} }, //used internally
    { muxI12.pin(4), {0x5C, Channel::createChannel(synthMidiChannel)} },
    { muxI12.pin(5), {0x5D, Channel::createChannel(synthMidiChannel)} }//,
    //{ muxI12.pin(6), {0x5E, Channel::createChannel(synthMidiChannel)} }, //PINS NOT CONNECTED
    //{ muxI12.pin(7), {0x5F, Channel::createChannel(synthMidiChannel)} },
};

// all used internally
// CCPotentiometer I13_POTS[] {
//     { muxI13.pin(0), {0x60, Channel::createChannel(synthMidiChannel)} },
//     { muxI13.pin(1), {0x61, Channel::createChannel(synthMidiChannel)} },
//     { muxI13.pin(2), {0x62, Channel::createChannel(synthMidiChannel)} },
//     { muxI13.pin(3), {0x63, Channel::createChannel(synthMidiChannel)} },
//     { muxI13.pin(4), {0x64, Channel::createChannel(synthMidiChannel)} },
//     { muxI13.pin(5), {0x65, Channel::createChannel(synthMidiChannel)} },
//     { muxI13.pin(6), {0x66, Channel::createChannel(synthMidiChannel)} },
//     { muxI13.pin(7), {0x67, Channel::createChannel(synthMidiChannel)} },
// };


///
/// Variables
///

// general Variables

IntervalTimer quickReadTimer;

const int numTriggerFrequencies = 14; //number of trigger frequencies
const int clockSubTicks = 24; //how many subticks in a clock

// !!! sets the size of each row. used when looping through the array
// !!! change this if you change triggerTable
const int rowSizes[] = {1, 1, 2, 3, 2, 3, 4, 3, 4, 5, 6, 8, 12, 24};
//at which subtick of the clock the subclock shall be triggered
int triggerTable[numTriggerFrequencies][clockSubTicks] = {
  {0}, //1
  {12}, //1 off
  {0, 12}, //2
  {0, 8, 16}, //3
  {8, 16}, //3 Jump
  {0, 8, 20}, //3 Swing
  {0, 6, 12, 18}, //4
  {6, 12, 18}, //4 Jump
  {0, 8, 12, 20}, // 4 Swing
  {0, 8, 12, 16, 20}, //6 Jump
  {0, 4, 8, 12, 16, 20}, // 6
  {0, 3, 6, 9, 12, 15, 18, 21}, //8
  {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22}, // 12
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23} // 24
};


float currentMicros = 0; //buffers the measured Microsecond timestamp at the start of the loop

//for benchmark purposes
int startLoopMicros = 0;
int controlSurfaceMicros = 0;
int drumPadMicros = 0;
int analogReadMicros = 0;
int digitalReadMicros = 0;
int updateValueMicros = 0;
int seqNotesMicros = 0;
int endLoopMicros = 0;


//Struct variables

// all general variables of the sequencer that are not bound to an input
//const = global settings
struct sequencer {
  const uint8_t velocity = 127; // standard velocity for notes
  const long bpmMin=5.0; //minimum BPM value (for mapping)
  const long bpmMax=400.0; //maximum BPM value (for mapping)
  int gateTime = 50; //time in milliseconds how long the note should be on
  const int gateMin = 5; //minimum gate time in milliseconds for pot selection
  const int gateMax = 1000; //maximum gate time in milliseconds for pot selection
  int slideAmount = 0; //how much note slide if enabled MidiCC value from 0-127
  int noteRange = 60; //range of notes the sequencer goes up from the min note. bound to pot
  int minSeqNote = 20; //minimal Midi Note of sequencer fader, bound to pot
  int maxSeqNote = minSeqNote + noteRange; //maximum sequencer note. minimum note + range
  const int minPulse = 1; //how many pulses at least for sequencer
  const int maxPulse = 8; //how many pulses at max for sequencer
  const int maxGateMode = 2; //How Many gate Modes there are (0-indexed)
  bool run = false; // if sequencer is running
  bool reset = false; //if reset was pressed
  const int maxStepCount = 8; //How many steps the sequencer has
  int numSteps = maxStepCount; // how many steps should be done. Jumps back to first step after numSteps
  int firstStep = 0; //first sequencer step to be played (for hold mode)
  int lastStep = numSteps; //last sequencer step to be played (for hold mode). first and last can be identical
  int seqDirection = 1; // Sequencer step direction. 1=up, 0=down
  int pulsePointer = 1; //points to the pulse within the step we're currently in
  int lastStepPointer = 0; //previous step, to trigger note off
  int lastNoteSent = 0; //previous note sent, to trigger note off on time
  int playMode = 0; // Sequencer play order. 0=forwards, 1=backwards, 2=ping-pong,3=drunk
  const int numPlayModes = 4; // how many play modes there are of the sequencer
  bool fuSel0 = 0; // Sequencer button Function Selectors. Act as 2bit input field to select
  bool fuSel1 = 0; // what the sequencer buttons do. 
  int stepPointer = 0; //pointer, points to current step that we're at
  int buttonFunction = 0; //Int representation of the selected function. 0=Play, 1=Skip, 2=Slide, 3=hold
  // 0=play/trigger pulse/step
  // 1=enable/disable skip step
  // 2=enable/disable slide step
  // 3=?? open for ideas: hold button=skip all others,play only this note | play between the two held buttons | arp mode
};
//constant values below
const int maxSteps = 8; // Maximum number of Steps of your sequencer
const int minAnalog = 2; //minimum readings of analog inputs
const int maxAnalog = 4088; //maximum readings of analog inputs. both used for mapping

// all variables that apply to every sequencer step
struct seqStep {
  int pulseCount = 1; // how many times a step should be played
  int gateMode = 2; //0=no gate, 1=first gate, 2=every gate
  bool skip = 0; //1=skip a step, 0=don't skip
  bool slide = 0; //1=slide, 0= don't slide
  int note = 0; //Note or Value of the Slider
  bool hold = 0; //for hold mode, if note shall be sent individually
};

//all variables for the main clock
struct mainClock {
  bool run = false;
  int bpm = 60.0;
  int oldBPM = 0; //store last bpm to see if we actually had a bpm change
  bool bpmChanged = false; //set true if bpm changed, hence update tempo
  float tempo = 1000000.0/(bpm/60.0); //bpm in microseconds
  const int subTicks = clockSubTicks; //in how many ticks one beat shall be divided
  float tickMS = tempo/subTicks; //how long a tick is in ms
  int currentTick = 0; // which tick we're currently at (pointer)
  int lastTick = 0; //which tick we were at before (pointer)
  float lastTickTime = 0; //timestamp when last tick happened
  float timeSinceLastTick = 0; //to calculate if more than 1 tick happened since last calculation
  int missedTicks = 0; //missed ticks if clock not updated fast enough
  float clockStart = 0; //microsecond timestamp when current Clock Cycle was started
  float prevClockStart = 0; //previous microsecond timestamp when last clock signal was sent
  float elapsedTime = 0; //how many ms have passed since clock start -> suggested by chatGPT
  bool initialClockReset = true; //used to reset clock at first occurrence, to sync everything
  int initialClockResetTime = 1000; // reset clock after n ms of running to sync everything
  int selectedSubClock = 0; // which subclock is currently selected (which drum note has been triggered last)
  bool reactToExtClock = false; //trigger next clock cycle if midi Clock is received (untested)
  bool clockSource = false; //true = external clock is currently used, false = internal clock is currently used
  float lastExtClockTick = 0; //timestamp when last external clock was received
  int avgExtClockInterval = 0; //average interval of ext. clocks received. to continue if some are missed
  int extTicksPerBeat = 6; //how many ticks per bpm.
  int extTicksCounter = 0; //number of ticks received externally within this beat
  float extClockTimeout = 4000; //how many ms to wait until we switch from external to internal clock
  int extClockMissedCounter = 0; //counts missed clock cycles
};
//const values below
  const int numSubClocks = 6; //how many subClocks with individual mult/division

//all variables that every subclock has
struct subClock {
  int index = 0; //index in the subClocks array
  int ratio = 1; //ratio multiplied or divided from mainClock
  bool divMult = 0; //1 = division, 0 = multiplication
  int tick = 0; //after how many ticks it should be triggered
  int delay = 0; //how many ticks it should be delayed (off-beat)
  int ticksLeft = 0; //how many ticks left until trigger (tick + delay). not happening? ->$decrease every tick and set back to tick+delay after triggered
  int instrument = 10; //which instrument the clock is connected to (eg. seqencer, bassdrum, etc.) Higher than 10 is  sequencer instrument
  int midiChannel = 0; //which midichannel the subclock is sending to. only used for drums
  float gateTime = 75; //after how many milliseconds the stop signal should be triggered
  bool run = true; //1 = running, 0 = stopped
  bool isStart = 1; //1 = Note should be started, 0 = note should be stopped
  bool stopSent = 1; //1 = stop has been sent for a note already, 0 = stop not sent yet (otherwise it would spam stop always)
  float startMS = 0; //microsecond timestamp when Note start was sent
  float tickCounter = 0; //how many ticks have passed for subclock: increase every tick
  int delayBuffer = 0; //store actual delay value to restore when delay has been set to 0
  int triggerFrequency = 0; //Which trigger frequency from the triggerTable[] is selected
  float triggerProb = 1; // probability of the trigger actually occuring
};

///
/// DRUM Variables
///

const int numDrumInstruments = 4; //(0=kick, 1=snare, 2=highhat, 3=cymbal)

//stores midi note to be sent for every drum instrument
int drumInstrumentNotes[numDrumInstruments] = {
  63, //Kick
  67, //snare
  71, //highhat
  75  //cymbals
};

//all variables that every drumpad has
struct drumPad {
  int drumMidiChannel = 13; //Midi channel to which the keypad notes are sent
  int keypadMode = 1; //1 = play, 2 = setRate, 3 = Perform, 4 = enable/disable mode
  const int numKeypadModes = 4;
};



sequencer Metropolis[1]; //initializes a struct with the sequencer variables
seqStep seqSteps[maxSteps]; //initializes a struct with the variables for each step. length defined by maxSteps of Sequencer
mainClock mainClocks[1]; //the main clock is initialized
subClock subClocks[numSubClocks]; //initialize subClocks based on number in mainClock
drumPad telephone[1]; //initialize drumpad, called telephone because i^m using an old telephone keypad

///
///
///
/// NEXT
/// make drum hits configurable via keypad
/// and also playable (ratchets)
///

//sets the default settings of all subclock/instruments
void setDefaultClockSettings(){

  //Default subClock Settings
  subClocks[0].index = 0; //sequencer
  subClocks[1].index = 1; //Kick
  subClocks[2].index = 2; //closed hat
  subClocks[3].index = 3; //open hat
  subClocks[4].index = 4; //snare
  subClocks[5].index = 5; //fast clock for korg volca

  subClocks[0].ratio = 4;
  subClocks[1].ratio = 1;
  subClocks[2].ratio = 3;
  subClocks[3].ratio = 8;
  subClocks[4].ratio = 2;
  subClocks[5].ratio = 24;

  subClocks[0].divMult = 1;
  subClocks[1].divMult = 1;
  subClocks[2].divMult = 1;
  subClocks[3].divMult = 1;
  subClocks[4].divMult = 0;
  subClocks[5].divMult = 1;

  // when the subclock shall be triggered
  subClocks[0].triggerFrequency = 0;
  subClocks[1].triggerFrequency = 0;
  subClocks[2].triggerFrequency = 1;
  subClocks[3].triggerFrequency = 0;
  subClocks[4].triggerFrequency = 0;
  subClocks[5].triggerFrequency = 0;

//not intuitive
// 0-9 are drum instruments, so are not stopped automatically, see loop
// only some instrument indexes (0-3) exist in the drumInstrumentNotes array below
  subClocks[0].instrument = 12;
  subClocks[1].instrument = 0;
  subClocks[2].instrument = 1;
  subClocks[3].instrument = 2;
  subClocks[4].instrument = 3;
  subClocks[5].instrument = 4;

  //corresponding to part number on korg volca for drum instruments
  subClocks[0].midiChannel = 12; //not used
  subClocks[1].midiChannel = 1;
  subClocks[2].midiChannel = 2;
  subClocks[3].midiChannel = 3;
  subClocks[4].midiChannel = 4;
  subClocks[5].midiChannel = 5;

  subClocks[0].gateTime = 75;
  subClocks[1].gateTime = 20;
  subClocks[2].gateTime = 20;
  subClocks[3].gateTime = 20;
  subClocks[4].gateTime = 20;
  subClocks[5].gateTime = 20;

  subClocks[0].run = 1;
  subClocks[1].run = 1;
  subClocks[2].run = 1;
  subClocks[3].run = 0;
  subClocks[4].run = 0;
  subClocks[5].run = 0;
}





// FilteredAnalog<12,3,uint32_t> testAnalogInput[] {
//   { muxI8.pin(0)},
//   { muxI8.pin(1)},
//   { muxI8.pin(2)},
//   { muxI8.pin(3)},
//   { muxI8.pin(4)},
//   { muxI8.pin(5)},
//   { muxI8.pin(6)},
//   { muxI8.pin(7)}
// };

//all potentiometers that don't send midi directly
//but are used for internal variables
FilteredAnalog<12,3,uint32_t> internalAnalog[] {
    { muxI3.pin(2)}, //BPM
    { muxI3.pin(3)}, //numsteps
    { muxI3.pin(4)}, //GateTime
    { muxI3.pin(5)}, // Slide Amount
    { muxI3.pin(6)}, // Max Note
    { muxI3.pin(7)}, // Min Note
    { muxI12.pin(2)}, //Keypad Mode
    { muxI12.pin(3)} //Sequencer Rate
};

// IMPORTANT: insert in the order they will be used in the sequencer
// first pot at [0], second pot at [1] etc...
FilteredAnalog<12,3,uint32_t> seqPulseCountPot[] {
    { muxI6.pin(0)},
    { muxI6.pin(1)},
    { muxI6.pin(2)},
    { muxI6.pin(3)},
    { muxI6.pin(4)},
    { muxI6.pin(5)},
    { muxI6.pin(6)},
    { muxI6.pin(7)}
};

FilteredAnalog<12,3,uint32_t> seqSliderPot[] {
    { muxI5.pin(7)},
    { muxI5.pin(6)},
    { muxI5.pin(5)},
    { muxI5.pin(4)},
    { muxI5.pin(3)},
    { muxI5.pin(2)},
    { muxI5.pin(1)},
    { muxI5.pin(0)}
};

FilteredAnalog<12,3,uint32_t> seqGateModePot[] {
    { muxI13.pin(0)},
    { muxI13.pin(1)},
    { muxI13.pin(2)},
    { muxI13.pin(3)},
    { muxI13.pin(4)},
    { muxI13.pin(5)},
    { muxI13.pin(6)},
    { muxI13.pin(7)}
};

//all buttons (digital inputs) that don't send midi directly
//but are used for internal variables
Button internalDigital[] {
  { muxI4.pin(0)}, //Start/stop
  { muxI4.pin(1)}, //reset
  { muxI4.pin(2)}, //playmode
  { muxI4.pin(3)}, //clear
  { muxI4.pin(4)}, //fusel0
  { muxI4.pin(5)}  //fusel1
};

// Button objects, digital inputs of the Sequencer buttons
Button seqButtons[] {
  { muxI8.pin(7)},
  { muxI8.pin(6)},
  { muxI8.pin(5)},
  { muxI8.pin(4)},
  { muxI8.pin(3)},
  { muxI8.pin(2)},
  { muxI8.pin(1)},
  { muxI8.pin(0)}
};

///
/// Keypad
///
const AddressMatrix<4, 4> keypadNotes = {{
  {60, 61, 62, 63},
  {64, 65, 66, 67},
  {68, 69, 70, 71},
  {72, 73, 74, 75}
}};


const NoteButtonMatrix<4, 4> keypadMatrix {
  {5,6,12,11}, //output LOW
  {7,8,9,10}, //input pullup
  keypadNotes,
  Channel::createChannel(telephone[0].drumMidiChannel)
};



///
///Functions
///

//Function Prototypes
void selectSeqNoteFunction(void);
void startNote(int);
void stopNote(int);
void noteButtonPressed(int);
void noteButtonReleased(int);
int getCurrentTick(void);
void resetClock(void);
void resetSequencer(void);
void stopLastNote(void);
void startDrumNote(int);
void stopDrumNote(int);
void readDrumPad(void);

// selects the pin on output multiplexer (LEDs)
void selectMuxOutPin(byte pin){
  //make sure the value is not negative and not over 8
  //pin = constrain(pin, 0, 8);
  pin = map(pin, 0, 7, 7, 0); //invert value as LEDs are backwards
  //pin = Metropolis[0].maxStepCount-pin; //LEDs are backwards, so adressing them backwards
  // Serial.print("calculated ");
  // Serial.println(pin);
  bool valA = bitRead(pin, 0);
  bool valB = bitRead(pin, 1);
  bool valC = bitRead(pin, 2);
  // Serial.print(valA);
  // Serial.print(valB);
  // Serial.println(valC);
  digitalWrite(Aout, valA);
  digitalWrite(Bout, valB);
  digitalWrite(Cout, valC);
}

/// reads/updates all internal analog and digital inputs
void readInternalInputs() {

  //analog
  for (byte i = 0; i < (sizeof(internalAnalog) / sizeof(internalAnalog[0])); i++) {
      internalAnalog[i].update();
    }

  // for (byte i = 0; i < (sizeof(testAnalogInput) / sizeof(testAnalogInput[0])); i++) {
  //     testAnalogInput[i].update();
  // }

  for (byte i = 0; i < (sizeof(seqPulseCountPot) / sizeof(seqPulseCountPot[0])); i++) {
      seqPulseCountPot[i].update();
  }

  for (byte i = 0; i < (sizeof(seqSliderPot) / sizeof(seqSliderPot[0])); i++) {
      seqSliderPot[i].update();
  }

  for (byte i = 0; i < (sizeof(seqGateModePot) / sizeof(seqGateModePot[0])); i++) {
      seqGateModePot[i].update();
  }

// Digital
  for (byte i = 0; i < (sizeof(internalDigital) / sizeof(internalDigital[0])); i++) {
      internalDigital[i].update();
    }

  for (byte i = 0; i < (sizeof(seqButtons) / sizeof(seqButtons[0])); i++) {
    seqButtons[i].update();
  }
    
}



/// stores read values to the correct variables
void UpdateInternalVars(){

  //Metropolis[0];

  //update bpm only if no external clock is receiced
  if (mainClocks[0].clockSource == false){

    //update bpm only if it changed
    if (mainClocks[0].bpm != map(internalAnalog[0].getValue(),minAnalog,maxAnalog,Metropolis[0].bpmMin,Metropolis[0].bpmMax)){
    mainClocks[0].bpm = map(internalAnalog[0].getValue(),minAnalog,maxAnalog,Metropolis[0].bpmMin,Metropolis[0].bpmMax);
    mainClocks[0].bpmChanged = true;
    // Serial.print("BPM: ");
    // Serial.println(mainClocks[0].bpm);
    }
    else {
      mainClocks[0].bpmChanged = false;
    }
  }


  Metropolis[0].numSteps = map(internalAnalog[1].getValue(),minAnalog,maxAnalog,1,Metropolis[0].maxStepCount);
  
  //set gatetime for sequencer and for subclock
  Metropolis[0].gateTime = map(internalAnalog[2].getValue(),minAnalog,maxAnalog,Metropolis[0].gateMin,Metropolis[0].gateMax);
  subClocks[0].gateTime = map(internalAnalog[2].getValue(),minAnalog,maxAnalog,Metropolis[0].gateMin,Metropolis[0].gateMax);
  
  Metropolis[0].slideAmount = map(internalAnalog[3].getValue(),minAnalog,maxAnalog,0,127);

  //set the note range of the sequencer
  //define 
  Metropolis[0].minSeqNote = map(internalAnalog[5].getValue(),minAnalog,maxAnalog,0,127); //minimum note highest -8
  Metropolis[0].noteRange = map(internalAnalog[4].getValue(),minAnalog,maxAnalog,0,127); //maximum note lowest +8
  
  // Keypad Mode of the telephone pad
  telephone[0].keypadMode = map(internalAnalog[6].getValue(),minAnalog,maxAnalog,1,telephone[0].numKeypadModes);
  Serial.println(telephone[0].keypadMode);
  /// *** now using trigger frequency
  // ratio of the sequencer
  //subClocks[0].ratio = map(internalAnalog[7].getValue(),minAnalog,maxAnalog,1,16);
  /// *** now

  // set triggerFrequency of sequencer
  subClocks[0].triggerFrequency = map(internalAnalog[7].getValue(),minAnalog,maxAnalog,0,numTriggerFrequencies-1);

  //calculate max Sequencer note
  Metropolis[0].maxSeqNote = Metropolis[0].minSeqNote + Metropolis[0].noteRange;

  //restrict the max. note to 127, maximum midi note
  if (Metropolis[0].maxSeqNote > 127) {
    Metropolis[0].maxSeqNote = 127;
  }
  // Serial.print(Metropolis[0].minSeqNote);
  // Serial.print("->");
  // Serial.println(Metropolis[0].maxSeqNote);
  
  if (internalDigital[0].getState() == Button::State::Falling) {
    Metropolis[0].run = !Metropolis[0].run; // Toggle Start/Stop
    mainClocks[0].run = !mainClocks[0].run; //toggle clock run

    if (Metropolis[0].run){
      // Serial.println("play")
      resetClock();
      usbMIDI.sendStart();
    } else {
      // Serial.println("pause"); #
      stopLastNote();
      stopNote(Metropolis[0].stepPointer);
      stopAllNotes();
      usbMIDI.sendStop();
    }
  }
  if (internalDigital[1].getState() == Button::State::Falling){
    Metropolis[0].reset = true;
  }

  if (internalDigital[2].getState() == Button::State::Falling){
    Metropolis[0].playMode++; //increment play mode
    if (Metropolis[0].playMode >= Metropolis[0].numPlayModes){ //wrap play mode if max reached
      Metropolis[0].playMode = 0;
    }
  }

  //switches inverted
  Metropolis[0].fuSel0 = !internalDigital[4].getState(); 
  Metropolis[0].fuSel1 = !internalDigital[5].getState();

  //seqSteps[0];

  for (int i = 0; i < Metropolis[0].maxStepCount; i++){
    seqSteps[i].pulseCount = map(seqPulseCountPot[i].getValue(),minAnalog,maxAnalog,Metropolis[0].minPulse,Metropolis[0].maxPulse);
    seqSteps[i].gateMode = map(seqGateModePot[i].getValue(),minAnalog,maxAnalog,0,Metropolis[0].maxGateMode);
    seqSteps[i].note = map(seqSliderPot[i].getValue(),minAnalog,maxAnalog,Metropolis[0].minSeqNote,Metropolis[0].maxSeqNote);
  }

  //mainClocks[0];

  //Sequencer Buttons
  for (int i = 0; i < Metropolis[0].maxStepCount; i++){
    switch (Metropolis[0].buttonFunction) {
      case 0: //Play
          //only play notes when sequencer is not running, and not synced to ext clock
          if (!Metropolis[0].run) {//&& !syncClockToExt
            if (seqButtons[i].getState() == Button::State::Falling){
              startNote(i);
            }
            if (seqButtons[i].getState() == Button::State::Rising){
              stopNote(i);
            }
          }
      break;

      case 1: //Skip
        
        //Clear values if "clear" pressed
        if (internalDigital[3].getState() == Button::State::Falling){
         seqSteps[i].skip = false;
        }

        if (seqButtons[i].getState() == Button::State::Falling) {
            seqSteps[i].skip = !seqSteps[i].skip;
          }
      break;

      case 2: //Slide
        //Clear values if "clear" pressed
        if (internalDigital[3].getState() == Button::State::Falling){
          seqSteps[i].slide = false;
        }

        if (seqButtons[i].getState() == Button::State::Falling) {
          seqSteps[i].slide = !seqSteps[i].slide;
        }
      break;

      case 3: //hold

        //Clear values if "clear" pressed
        if (internalDigital[3].getState() == Button::State::Falling){
          seqSteps[i].hold = false;
        }

        if (seqButtons[i].getState() == 0) {
          seqSteps[i].hold = true;
        } else {
          seqSteps[i].hold = false;
        }
      break;
    }
  }

  //set first and last step
  if (Metropolis[0].buttonFunction == 3) { //hold mode
    int holdButtonCount = 0;
    //get the first held button
    for (int i = 0; i < Metropolis[0].numSteps; i++){
      if (seqSteps[i].hold) {
        if (holdButtonCount == 0) { //only do on the first held button
          Metropolis[0].firstStep = i;
        }
        holdButtonCount++;
      }
    }

    //get the last held button
    for (int i = 0; i < Metropolis[0].numSteps; i++){
      if (seqSteps[i].hold) {
        Metropolis[0].lastStep = i+1; //don't return, so last held button will be lastStep
        //+1 as steps are 1 based, not 0 based
      }
    }

    if (holdButtonCount == 1) { //if only 1 button held, first and last step are same, no +1
      Metropolis[0].lastStep = Metropolis[0].firstStep;

      //buggy corrective measures
      switch (Metropolis[0].playMode){
        case 1: //backwards
          Metropolis[0].lastStep += 1;
          Metropolis[0].firstStep += 1;
        break;

        case 2: //pingpong
          Metropolis[0].lastStep += 2;
          Metropolis[0].firstStep += 2;
        break;
      }
      
    }

    if (holdButtonCount < 1) { //if no buttons held, set to default values
      Metropolis[0].firstStep = 0;
      Metropolis[0].lastStep = Metropolis[0].numSteps;
    }
  } else //set first and last step to defaults
  {
    Metropolis[0].firstStep = 0;
    Metropolis[0].lastStep = Metropolis[0].numSteps;
  }

}

//still buggy fires all the time
void checkForMissedClocks() {
    int now = micros();
    if (mainClocks[0].run){
      if (mainClocks[0].avgExtClockInterval > 0){
        //if 1.5x of the expected clock interval passes
        if ((now - mainClocks[0].lastExtClockTick) > (mainClocks[0].avgExtClockInterval * 1.5)) {
        mainClocks[0].extClockMissedCounter++;
        //set last tick to now minus a half interval. acts as if the last tick had occured at the
        //expected average time
        mainClocks[0].lastExtClockTick = now - (mainClocks[0].avgExtClockInterval / 2);

        handleClock();
        Serial.print("missed! last: ");
        Serial.print(mainClocks[0].lastExtClockTick);
        Serial.print(" avg: ");
        Serial.print(mainClocks[0].avgExtClockInterval);
        Serial.print(" now: ");
        Serial.println(now);
        }
      }
    }
    
}

//executed when midi stop signal received
void handleStop(){
  Serial.println("stop");
  mainClocks[0].run = false;
  mainClocks[0].avgExtClockInterval = 0;
}

//executed when midi start signal received
void handleStart(){
  Serial.println("start");
  mainClocks[0].run = true;
}

//executed when midi reset signal received
void handleReset(){
  Serial.println("reset");
  mainClocks[0].avgExtClockInterval = 0;
  mainClocks[0].lastExtClockTick = micros();
  handleClock();
}

// what happens when an external clock signal is received
void handleClock() {
  
  int now = micros();
  if (mainClocks[0].run && mainClocks[0].reactToExtClock){
    if (mainClocks[0].lastExtClockTick > 0) {
      int interval = now - mainClocks[0].lastExtClockTick; // calculate interval between pulses
      if (mainClocks[0].avgExtClockInterval == 0) {
        mainClocks[0].avgExtClockInterval = interval;
      } else {
        //set interval to average of last 8 ticks. should smooth out missed ticks
        mainClocks[0].avgExtClockInterval = (mainClocks[0].avgExtClockInterval * 7 + interval) / 8;
      }
    }

    mainClocks[0].lastExtClockTick = now;
    mainClocks[0].extClockMissedCounter = false;


    mainClocks[0].extTicksCounter++;
    //only trigger every x tick
    if (mainClocks[0].extTicksCounter >= mainClocks[0].extTicksPerBeat){
      mainClocks[0].extTicksCounter = 0;

      //do the actual clock cycle
      usbMIDI.sendClock();
      nextPulse();
    }
  }
  
  
  //Serial.println("p");
}

void setup() {
  Control_Surface.begin();
  midi.begin();
  Serial.begin(9600);
  setDefaultClockSettings();

  //setup the LED Pin and Multiplexer
  pinMode(I7, OUTPUT);
  digitalWrite(I7, HIGH);
  pinMode(Aout, OUTPUT);
  pinMode(Bout, OUTPUT);
  pinMode(Cout, OUTPUT);

  for (int i = 0; i < numSubClocks; i++) {
    subClocks[i].delayBuffer = subClocks[i].delay;  // Store set delay to delay buffer
  }
  selectMuxOutPin(Metropolis[0].stepPointer);
  usbMIDI.setHandleClock(handleClock);
  usbMIDI.setHandleStart(handleStart);
  usbMIDI.setHandleStop(handleStop);
  usbMIDI.setHandleSystemReset(handleReset);
  //setup interrupt timer, calls function every 250 microseconds, even when other stuff is running
  //change this value if getting issues with controlsurface loop
  quickReadTimer.begin(anythingAnytimeAllAtOnce, 250);
}

void loop() {

  readDrumPad();
  
  startLoopMicros = micros();
  Control_Surface.loop();
  controlSurfaceMicros = micros();
  
  readInternalInputs();
  analogReadMicros = micros();

  UpdateInternalVars();
  updateValueMicros = micros();
  
  selectSeqNoteFunction();

  if (Metropolis[0].reset) {
    resetSequencer();
    resetClock();
  }

//stop notes if necessary (in case clock is not running)
  for (int i = 0; i < numSubClocks; i++){
    // divide by 1000 as gatetime is in milliseconds and currentmicros and startms are in microseconds
    if ( (currentMicros / 1000) > ((subClocks[i].startMS / 1000) + subClocks[i].gateTime)) { //if gate time is over
      if (subClocks[i].stopSent == 0) { //stop not sent
        if (subClocks[i].instrument >= 12 && Metropolis[0].run){ //sequencer instrument and sequencer running
          stopLastNote(); 
        }
        else // drum instrument
        {
          //stopDrumNote(instrumentNotes[1][int(subClocks[i][6])],(i-1)); //hacky, i-1 only currently is correct
        }
        
      }
    }
  }
  // for (int i = 0; i < 8; i++){
  //   Serial.print(seqButtons[i].getState());
  //   // Serial.print("->");
  //   // Serial.print(mainClocks[0].bpm);
  //    Serial.print(" | ");

  // }


  if (mainClocks[0].initialClockReset && currentMicros > mainClocks[0].initialClockResetTime){
    mainClocks[0].initialClockReset = false;
    resetClock();
  }

  //set clock source to internal when timeout is reached
  // if (currentMicros - mainClocks[0].lastExtClockTick > mainClocks[0].extClockTimeout)
  // {
  //   mainClocks[0].clockSource = false;
  //   Serial.print(currentMicros - mainClocks[0].lastExtClockTick);
  //   Serial.println(" switch to internal clock");

  // }
  //debugVars();
  endLoopMicros = micros();
  //debugTimer();
  //benchmark();
}

//calls all actions that need to be called frequently
void anythingAnytimeAllAtOnce(){
  updateClock();
  usbMIDI.read();
  midi.read();
  midi.update();
  //checkForMissedClocks(); //buggy asf
}

//displays the benchmark values to see which steps take how long
void benchmark() {
  Serial.print("Control Surface: ");
  Serial.println(controlSurfaceMicros-startLoopMicros);
  Serial.print("Updated all Values ");
  Serial.println(updateValueMicros-startLoopMicros);
  Serial.print("Full loop ");
  Serial.println(endLoopMicros-startLoopMicros);
}

void debugTimer(){
  Serial.print("BPM");
  Serial.print(mainClocks[0].bpm);
  Serial.print(" run: ");
  Serial.print(mainClocks[0].run);
  Serial.print(" tempo: ");
  Serial.print(mainClocks[0].tempo);
  Serial.print(" currentTick: ");
  Serial.print(mainClocks[0].currentTick);
  Serial.print(" missed Ticks: ");
  Serial.print(mainClocks[0].missedTicks);
  Serial.print(" | ");
  Serial.println();
}
void debugVars(){
  Serial.print(Metropolis[0].run);
  Serial.print(" | ");
  Serial.print(mainClocks[0].bpm);
  Serial.print(" | ");
  Serial.print(Metropolis[0].reset);
  Serial.print(" | ");
  Serial.print(Metropolis[0].playMode);
  Serial.print(" | ");
  Serial.print(Metropolis[0].numSteps);
  Serial.print(" | ");
  Serial.print(Metropolis[0].gateTime);
  Serial.print(" | ");
  Serial.print(Metropolis[0].fuSel1);
  Serial.print(" | ");
  Serial.print(Metropolis[0].fuSel0);
  Serial.print(" | ");
  Serial.print(seqSteps[0].pulseCount);
  Serial.print(" | ");
  Serial.print(seqSteps[0].note);
  Serial.print(" | ");
  Serial.print(seqSteps[0].gateMode);
  Serial.print(" | ");
  Serial.print(internalDigital[3].getState());
  Serial.print(" | ");
  Serial.print(seqButtons[0].getState());
  Serial.print(" | ");
  Serial.print(Metropolis[0].buttonFunction);
  Serial.print(" | ");
  Serial.print(seqSteps[0].skip);
  Serial.print(" | ");
  Serial.print(seqSteps[0].slide);
  Serial.print(" | ");
  Serial.print(seqSteps[0].hold);
  Serial.print(" | ");
  Serial.println();
}