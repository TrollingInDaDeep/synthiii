//sequencer
//const int note = 42;

//bool syncSequencerToClock = 0; // 1 = next sequencer note triggered on ext clock signal | 0 = internal bpm used as step tempo
//bool syncDrumToSequencer = 1; //1 = drum step triggered when sequencer steps | 0 = external clock received
//bool drumDivMultMode = false; // true = trigger drum steps, when "fill each n step" is activated
bool blnTapTempo = true; //1 = playMode button becomes tapTempo button 
///
/// Variable definitions
///
ArduinoTapTempo tapTempo;

int analogReadDiff = 0;
int caseNumber = 0;
int noteNumberDigiRead = 0;
int noteNumberAnalog = 0;



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
/// Functions
///

//Function Prototypes
void nextDrumStep(void);
int youFuckenKiddingMe(char);
int youFuckenKiddingMeToo(char);

void updateDrumModifier(int);
void changeDrumPattern(bool);
void recordKey(int, int);
int getDrumNote(int);
int getDrumIndex(int);

int getSubClockIndexByInstrument(int);
              case 43:
              if (blnTapTempo){
                  tapTempo.update(!arr_read_digital_inputs[0][2]);
                  bpm = long(tapTempo.getBPM());
                  Serial.println(bpm);
                }
                if (!blnTapTempo){
se 55:
              case 56:
                caseNumber = arr_send_digital_inputs[mux][pin];
                //-49, so 49 will be 0, 50 will be 1 etc...
                noteNumberDigiRead = caseNumber-49;

  //5 -> typo, but a sweet one :) stay a dreamer!


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
    
    
  