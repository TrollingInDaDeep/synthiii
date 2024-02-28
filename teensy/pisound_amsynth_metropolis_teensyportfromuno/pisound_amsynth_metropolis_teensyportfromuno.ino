//#include <MIDI.h>
#include <Timer.h>

// https://www.instructables.com/Old-School-Arduino-MIDI-Sequencer/

//MIDI_CREATE_DEFAULT_INSTANCE();

//vars
//timer
Timer t;                        // The timer is used to schedule MIDI events
//int tempo = 150;                // Tempo in Beats Per Minute
//unsigned long delayTime = 100;  // Delay between notes, computed from tempo
//int noteValue = 16;             // This is 4 for quarter notes, 8 for eighth notes, etc.
//boolean changeTiming = false;   // If true, the clock speed has been moved

// alternative timer
long bpm = 90.0;
long tempo = 1000.0/(bpm/60.0); //bpm in milliseconds

float prevPulseStart = 0; //previous millisecond timestamp before last pulse was started
float prevClockStart = 0; //previous millisecond timestamp before last clock signal was sent
float clockInterval = tempo/24; //number of milliseconds after which to send the midi clock. MIDI spec is to send clock 24 times per quarter note
float pulseStart = 0; //timestamp when the pulse started
float noteStart = 0; //timestamp when the note started

const int note = 42;
const uint8_t velocity = 127;

//int clockspeed = 120;
bool run = true;
bool reset = false;
int numSteps = 8; // how many steps should be done. Jumps back to first step after numSteps
const int maxSteps = 8; // Maximum number of Steps of your sequencer
int stepValues[maxSteps] {0, 0, 0, 0, 0, 0, 0, 0}; // initialize values to middle
bool digitalInputs1[8] {0, 0, 0, 0, 0, 0, 0, 0}; //first multiplexer with digital inputs
bool prevDigitalInputs1[8] {0, 0, 0, 0, 0, 0, 0, 0}; //previous state of the inputs

int pulsePointer = 0; //points to the pulse within the step we're currently in
int lastStepPointer = 0; //previous step, to trigger note off
int lastNoteSent = 0; //previous note sent, to trigger note off on time
int pulseCount [] {1, 1, 1, 1, 1, 1, 1, 1}; // how many times a step should be played
int gateMode [maxSteps] {2, 2, 2, 2, 2, 2, 2, 2}; //0=no gate, 1=first gate, 2=every gate
bool skipStep [maxSteps] {0, 0, 0, 0, 0, 0, 0, 0}; //1=skip a step, 0=don't skip

bool noteStopped = true; //if note has been stopped for this step already

int stepPointer = 0; //pointer, points to current step that we're at
int gateTime = 50; //time in ms how long the note should be on

// Pins
const int muxSeqIO = 25;// multiplexer with sequencer faders
const int muxLEDIO = 12; // multiplexer with sequencer leds
const int muxDigIO = 11; // multiplexer with digital inputs

//const int selectPins[3] = {8, 9, 10};//{10, 9, 8}; //A on mux is pin 10, B=9, C=8
const int muxIn_A = 37;
const int muxIn_B = 38;
const int muxIn_C = 39;

const int muxOut_A = 30;
const int muxOut_B = 31;
const int muxOut_C = 32;

// Step Pitch readings


// Step pulse count readings


// Step gate mode readings


// Step slide readings


// Step skip readings

/// very neat function from
/// https://forum.arduino.cc/t/multiplexer-cd4051be/515666
/// most elegant I found, but I dont understand it
/// << is a bitwise operator, something like that
void selectMuxInPin(byte pin) {

  bool valA = bitRead(pin, 0);
  bool valB = bitRead(pin, 1);
  bool valC = bitRead(pin, 2);

  digitalWrite(muxIn_A, valA);
  digitalWrite(muxIn_B, valB);
  digitalWrite(muxIn_C, valC);
  delay(1);
}

void selectMuxOutPin(byte pin){
  bool valA = bitRead(pin, 0);
  bool valB = bitRead(pin, 1);
  bool valC = bitRead(pin, 2);

  digitalWrite(muxOut_A, valA);
  digitalWrite(muxOut_B, valB);
  digitalWrite(muxOut_C, valC);
  delay(1);
}

void noteButtonPressed(int note) {
  startNote(note);
}

void noteButtonReleased(int note){
  stopNote(note);
}

// functions
void updateTempo(){
  tempo = 1000.0/(bpm/60.0);
  clockInterval = tempo/24;
}

void nextStep() {

  stepPointer++;

  //go to next step if all pulses are done
  if (stepPointer >= numSteps) {
    stepPointer = 0;
  }
  selectMuxOutPin(byte(stepPointer));
}


//buggy, not sure if needed
void stopLastNote(){
  usbMIDI.sendNoteOff(lastNoteSent, velocity, 1);
  noteStopped = true;

}

void startNote(int noteToPlay){
  stopLastNote();
  noteStart = millis();
  usbMIDI.sendNoteOn(stepValues[noteToPlay], velocity, 1);
  noteStopped = false;
  lastNoteSent=stepValues[noteToPlay];
  digitalWrite(muxLEDIO, HIGH);
}

void nextPulse() {
  //go to next pulse
  pulseStart = millis();

  if (skipStep[stepPointer]) {
    stepPointer++;
  }
  startNote(stepPointer); // start the note of this pulse


  pulsePointer++;

  //go to next step if last pulse is reached
  if (pulsePointer >= pulseCount[stepPointer]) {
    pulsePointer = 0;
    nextStep();
  }
}

void stopNote(int noteToStop){
  usbMIDI.sendNoteOff(stepValues[noteToStop], velocity, 1);
  noteStopped = true;
  //digitalWrite(muxLEDIO, LOW);
}

void readAnalogPins() {
  
  // read multiplexer
  for (byte pin=0; pin<=7; pin++){
    selectMuxInPin(pin);
    // Serial.print(pin);
    // Serial.println(" selected");
    //sequencer Mux values
    int seqRaw = analogRead(muxSeqIO);
    stepValues[pin] = map(seqRaw,0,1023,0,85);
    // Serial.print("reads ");
    // Serial.println(map(seqRaw,0,1023,24,83));
    //analog pins
    int potClockrawr = analogRead(A1);
    int potClock = map(potClockrawr, 0,1023,40,480);
    bpm = potClock;
    //gateTime = potClockrawr;
  }
}

void readDigitalPins() {
  
  // read multiplexer
  for (byte pin=0; pin<=7; pin++){
    selectMuxInPin(pin);
    //store current value in previous for comparison later
    prevDigitalInputs1[pin] = digitalInputs1[pin];

    //inversion because LOW is enabled
    digitalInputs1[pin] = !digitalRead(muxDigIO);

    //analog pins
    // int potClockrawr = analogRead(A1);
    // int potClock = map(potClockrawr, 0,1023,40,480);
    // bpm = potClock;
    //gateTime = potClockrawr;
  }
}

void sendDigitalPins(){
  //loop through digital inputs
  for (int ptr=0; ptr<=7; ptr++){
    //check if a value changed
    if (digitalInputs1[ptr] != prevDigitalInputs1[ptr]){
      switch(ptr){

        //button for sequencer note 1
        case 0:
          if (digitalInputs1[ptr]){
            noteButtonPressed(0);
          }
          if (!digitalInputs1[ptr]){
            noteButtonReleased(0);
          }
          break;
        //button for sequencer note 2
        case 1:
          if (digitalInputs1[ptr]){
              noteButtonPressed(1);
            }
            if (!digitalInputs1[ptr]){
              noteButtonReleased(1);
            }
            break;
        // skip button for step 3
        case 2:
            if(digitalInputs1[ptr]){
              skipStep[2]=!skipStep[2];
            }
          break;
        case 3:
          Serial.println(3);
          break;
        case 4:
          Serial.println(4);
          break;
        case 5:
          Serial.println(5);
          break;
        case 6:
          Serial.println(6);
          break;
        case 7:
          if (digitalInputs1[ptr]){
            run=!run;
          }
      
          break;
      }
    }
  }
  
}
void setup() {

  // for (int i = 0; i>3; i++){
  //   pinMode(selectPins[i], OUTPUT);
  // }

  pinMode(muxIn_A,OUTPUT);
  pinMode(muxIn_B,OUTPUT);
  pinMode(muxIn_C,OUTPUT);

  pinMode(muxOut_A,OUTPUT);
  pinMode(muxOut_B,OUTPUT);
  pinMode(muxOut_C,OUTPUT);

  pinMode(muxLEDIO, OUTPUT);
  pinMode(muxSeqIO, INPUT);
  pinMode(muxDigIO, INPUT_PULLUP);
  pinMode(A1, INPUT);

  //turn on the LED
  digitalWrite(muxLEDIO, HIGH);
  byte zerro = 0;
  selectMuxOutPin(zerro);

  Serial.begin(115200);
  usbMIDI.sendControlChange(123, velocity, 1);
  delay(10);
}

void loop() {

  unsigned long currentMillis = millis();
  
  readAnalogPins();
  readDigitalPins();
  sendDigitalPins();
  updateTempo();
  
  //make sure the gateTime doesn't exceed the tempo, note shouldnt be longer than
  //the pulse itself
  if (gateTime > tempo) {
    gateTime = tempo - 30; //30ms so that the note can end in peace before the next pulse is triggered
  }

  if (run) {
    // send clock if necessary
    if (currentMillis - prevClockStart >= clockInterval) {
      //save timestamp when pulse started
      prevClockStart = currentMillis;
      usbMIDI.sendClock();
    }

    // do next pulse if necessary
    if (currentMillis - prevPulseStart >= tempo) {
      //save timestamp when pulse started
      prevPulseStart = currentMillis;
      nextPulse(); //after last pulse, next step will be triggered
    }
    //stop note if necessary
    if (!noteStopped) {
      if (currentMillis - noteStart >= gateTime) {
        stopLastNote();
      }
    }
  }
  
    //MIDI.sendControlChange(1, pot, 1);
    
    //Serial.println(pot);
    //delay(beatDelay);
    //MIDI.read();
}

