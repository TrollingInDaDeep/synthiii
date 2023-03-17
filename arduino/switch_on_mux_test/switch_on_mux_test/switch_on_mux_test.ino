#include <Control_Surface.h>
HairlessMIDI_Interface midi;


// Ditital Pins
  const int Din = 1;

  const int T1_A = 11;
  const int T1_B = 12;
  const int T1_C = 13;

//CD74HC4051 D1 { Din, {T1_A, T1_B, T1_C} };
CD74HC4051 muxA1 {A0, {T1_A, T1_B, T1_C} };
//CCButtonLatching swDin {D1.pin(4), {12, CHANNEL_3}};
PBPotentiometer polPot { muxA1.pin(0), CHANNEL_9 };
//mux.pin(0), {MIDI_CC::Channel_Volume, CHANNEL_1}

void setup() {

  pinMode(A0, INPUT);
  pinMode(Din,INPUT_PULLUP);
  // put your setup code here, to run once:
  Serial.flush();
  Control_Surface.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  Control_Surface.loop();
}
