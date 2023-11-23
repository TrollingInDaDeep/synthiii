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
long bpm = 60.0;
long tempo = 1000.0/(bpm/60.0); //bpm in milliseconds

float prevPulseStart = 0; //previous millisecond timestamp before last pulse was started
//float interval = tempo/24.0; //number of milliseconds after which to send the midi clock. MIDI spec is to send clock 24 times per quarter note
float pulseStart = 0; //timestamp when the pulse started
float noteStart = 0; //timestamp when the note started

const int note = 42;
const uint8_t velocity = 127;

//int clockspeed = 120;
bool run = true;
bool reset = false;
int numSteps = 8; // how many steps should be done. Jumps back to first step after numSteps
const int maxSteps = 8; // Maximum number of Steps of your sequencer
int stepValues[maxSteps] {64, 68, 64, 55, 64, 64, 40, 64}; // initialize values to middle

int pulsePointer = 0; //points to the pulse within the step we're currently in
int lastPulsePointer = 0; //previous pulse, to trigger note off
int lastStepPointer = 0; //previous step, to trigger note off
int pulseCount [] {1, 1, 1, 1, 1, 1, 1, 1}; // how many times a step should be played
int gateMode [] {2, 2, 2, 2, 2, 2, 2, 2}; //0=no gate, 1=first gate, 2=every gate
bool noteStopped = true; //if note has been stopped for this step already

int stepPointer = 0; //pointer, points to current step that we're at
int gateTime = 50; //time in ms how long the note should be on

// Pins
const int muxSeqIO = A0;
const int selectPins[3] = {10, 9, 8}; //A on mux is pin 10, B=9, C=8


// Step Pitch readings


// Step pulse count readings


// Step gate mode readings


// Step slide readings


// Step skip readings

/// very neat function from
/// https://forum.arduino.cc/t/multiplexer-cd4051be/515666
/// most elegant I found, but I dont understand it
/// << is a bitwise operator, something like that
void selectMuxPin(byte pin)
{
  Serial.print("selecting pin ");
  Serial.println(pin);
  
  bool valA = bitRead(pin, 0);
  bool valB = bitRead(pin, 1);
  bool valC = bitRead(pin, 2);

  Serial.print(selectPins[0]);
  Serial.print("=");
  Serial.println(valA);

  Serial.print(selectPins[1]);
  Serial.print("=");
  Serial.println(valB);

  Serial.print(selectPins[2]);
  Serial.print("=");
  Serial.println(valC);

  digitalWrite(selectPins[0], valA);
  digitalWrite(selectPins[1], valB);
  digitalWrite(selectPins[2], valC);
  delay(1);
  // for (int i=0; i<3; i++)
  // {
  //   if (pin & (1<<i))
  //     digitalWrite(selectPins[i], HIGH);
  //   else
  //     digitalWrite(selectPins[i], LOW);
  // }
}

void noteButtonPressed(int note) {
  startNote(note);
}

void noteButtonReleaseed(int note){
  stopNote(note);
}

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


//buggy, not sure if needed
void stopLastNote(){
  MIDI.sendNoteOff(stepValues[lastStepPointer], velocity, 1);
}

void startNote(int noteToPlay){
  noteStart = millis();
  MIDI.sendNoteOn(stepValues[noteToPlay], velocity, 1);
  noteStopped = false;
  //stopLastNote();
}

void nextPulse() {
  //go to next pulse
  pulseStart = millis();
  lastPulsePointer = pulsePointer;

  startNote(stepPointer); // start the note of this pulse
  
  pulsePointer++;
  //go to next step if last pulse is reached
  if (pulsePointer >= pulseCount[stepPointer]) {
    pulsePointer = 0;
    nextStep();
  }
}

void stopNote(int noteToStop){
  MIDI.sendNoteOff(stepValues[noteToStop], velocity, 1);
  MIDI.sendControlChange(123, velocity, 1);
  noteStopped = true;
  nextPulse(); //after last pulse, next step will be triggered
}

void readAnalogPins() {
  
  // read multiplexer
  for (byte pin=0; pin<=7; pin++){
    selectMuxPin(pin);
    Serial.print(pin);
    Serial.println(" selected");
    //sequencer Mux values
    int seqRaw = analogRead(muxSeqIO);
    stepValues[pin] = map(seqRaw, 0, 1023, 24, 83);
    Serial.print("reads ");
    Serial.println(stepValues[pin]);
    //analog pins
    int potClockrawr = analogRead(A1);
    int potClock = map(potClockrawr, 0,1023,40,480);
    bpm = potClock;
    Serial.println("--------------------------");
  }
}

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);
  MIDI.sendControlChange(123, velocity, 1);

  for (int i = 0; i>3; i++){
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
  }

  pinMode(muxSeqIO, INPUT);
  pinMode(A1, INPUT);
}

void loop() {

  unsigned long currentMillis = millis();
  
  readAnalogPins();
  updateTempo();

  //make sure the gateTime doesn't exceed the tempo, note shouldnt be longer than
  //the pulse itself
  if (gateTime > tempo) {
    gateTime = tempo - 30; //30ms so that the note can end in peace before the next pulse is triggered
  }

  if (run) {
    if (currentMillis - prevPulseStart >= tempo) {
      //save timestamp when pulse started
      prevPulseStart = currentMillis;
      nextPulse();
    }
  }

    if (!noteStopped) {
      if (currentMillis - noteStart >= gateTime) {
        stopNote(stepPointer);
      }
    }
  
    //MIDI.sendControlChange(1, pot, 1);
    
    //Serial.println(pot);
    //delay(beatDelay);
    //MIDI.read();
}

