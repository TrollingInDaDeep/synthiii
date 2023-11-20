#include <MIDI.h>
#include <Timer.h>

// https://www.instructables.com/Old-School-Arduino-MIDI-Sequencer/

MIDI_CREATE_DEFAULT_INSTANCE();

//vars
//timer
Timer t;                        // The timer is used to schedule MIDI events
//int tempo = 150;                // Tempo in Beats Per Minute
//unsigned long delayTime = 100;  // Delay between notes, computed from tempo
//int noteValue = 16;             // This is 4 for quarter notes, 8 for eighth notes, etc.
//boolean changeTiming = false;   // If true, the clock speed has been moved

// alternative timer
long bpm = 130.0;
long tempo = 1000.0/(bpm/60.0); //bpm in milliseconds

float prevmillis = 0; //previous millisecond timestamp
//float interval = tempo/24.0; //number of milliseconds after which to send the midi clock. MIDI spec is to send clock 24 times per quarter note
float stepStart = 0; //timestamp when the step started

const int note = 42;
const uint8_t velocity = 127;

//int clockspeed = 120;
bool run = 1;
bool reset = 0;
int numSteps = 1; // how many steps should be done. Jumps back to first step after numSteps
const int maxSteps = 8; // Maximum number of Steps of your sequencer
int stepValues[maxSteps] {64, 68, 64, 55, 64, 64, 40, 64}; // initialize values to middle

int pulsePointer = 0; //points to the pulse within the step we're currently in
int lastPulsePointer = 0; //previous pulse, to trigger note off
int lastStepPointer = 0; //previous step, to trigger note off
int pulseCount [] {1, 1, 2, 3, 1, 1, 1, 1}; // how many times a step should be played
int gateMode [] {2, 2, 2, 2, 2, 2, 2, 2}; //0=no gate, 1=first gate, 2=every gate
bool noteStopped = true; //if note has been stopped for this step already

int stepPointer = 0; //pointer, points to current step that we're at
int gateTime = 100; //time in ms how long the note should be on

// Step Pitch readings


// Step pulse count readings


// Step gate mode readings


// Step slide readings


// Step skip readings

// functions
void updateTempo(){
  tempo = 1000.0/(bpm/60.0);
}

void nextStep() {
  //go to next step if all pulses are done
  lastStepPointer = stepPointer;
  stepPointer++;
  if (stepPointer >= numSteps) {
    stepPointer = 0;
  }
}
void nextPulse() {
  //go to next pulse
  lastPulsePointer = pulsePointer;
  pulsePointer++;
  if (pulsePointer >= pulseCount[stepPointer]) {
    pulsePointer = 0;
    nextStep();
  }
}

void stopLastNote(){
  MIDI.sendNoteOff(stepValues[lastStepPointer], velocity, 1);
}

void startNote(){
  stepStart = millis();
  MIDI.sendNoteOn(stepValues[stepPointer], velocity, 1);
  noteStopped = false;
  //stopLastNote();
}

void stopNote(){
  MIDI.sendNoteOff(stepValues[stepPointer], velocity, 1);
  MIDI.sendControlChange(123, velocity, 1);
  noteStopped = true;
  nextPulse(); //after last pulse, next step will be triggered
}


/////////////////////////////////////
// Read the tempo, and compute
// the delay between notes
// trigger the next note
//void adjustDelay()
// {
//   // Compute the delay time
//   int divisor = noteValue / 4;
//   long tempoMsec = 60000 / tempo;
//   if(divisor < 1)
//     delayTime = tempoMsec * (4 / noteValue);
//   else
//     delayTime = tempoMsec / divisor;
//   // Set the timer to trigger the first note in 10ms
//   t.after(delayTime, sendNote);
// }


/////////////////////////////////////
// Read the clock speed knob, compute the 
// tempo, and call the function
// that computes the delay
//  void setNewTempo()
//  {
//    // This sets a range of tempos from about 40 BPM to about 240 BPM
//    //tempo = 40 + ((1023 - pot1) / 5);
//    adjustDelay();
//  }


void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);
  MIDI.sendControlChange(123, velocity, 1);
}

void loop() {
    //t.update();  // Let the timer see if it's time to send a note
    // if(changeTiming){
    //     setNewTempo();
    // }else{
    //     adjustDelay();
    // }
    unsigned long currentMillis = millis();
    if (currentMillis - prevmillis >= tempo) {
      //save the last timestamp
      prevmillis = currentMillis;
      //Serial.println("startnote");
      startNote();
    }

    if (!noteStopped) {
      if (currentMillis - stepStart >= gateTime) {
        stopNote();
        // Serial.print("currentmillis: ");
        // Serial.println(currentMillis);
        // Serial.print("stepStart: ");
        // Serial.println(stepStart);
        // Serial.print("gateTime: ");
        // Serial.println(gateTime);
      }
    }
    int potraw = analogRead(A0);
    int pot2raw = analogRead(A1);
    int pot2 = map(pot2raw, 0,1023,40,480);
    int pot = map(potraw, 0, 1023, 24, 83);
    bpm = pot2;
    updateTempo();
    //stepValues[0] = pot;
    MIDI.sendControlChange(1, pot, 1);
    
    //Serial.println(pot);
    //delay(beatDelay);
    MIDI.read();
}

