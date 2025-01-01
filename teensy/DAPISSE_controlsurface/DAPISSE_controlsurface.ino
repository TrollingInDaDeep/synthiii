#include <Control_Surface.h>
#include <Arduino_Helpers.h>
#include <AH/Hardware/ExtendedInputOutput/AnalogMultiplex.hpp>
#include <AH/Hardware/FilteredAnalog.hpp>
// Instantiate a MIDI Interface to use
USBMIDI_Interface midi;

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
CD74HC4051 muxI1 {I1, {A, B, C} };
CD74HC4051 muxI2 {I2, {A, B, C} };
CD74HC4051 muxI3 {I3, {A, B, C} };
CD74HC4051 muxI4 {I4, {A, B, C} };
CD74HC4051 muxI5 {I5, {A, B, C} };
CD74HC4051 muxI6 {I6, {A, B, C} };
//CD74HC4051 muxI7 {I7, {Aout, Bout, Cout} };
CD74HC4051 muxI8 {I8, {A, B, C} };


// Controls
//CCPotentiometer
CCPotentiometer I1_POTS[] {
    { muxI1.pin(0), {0x00, Channel_1} },
    { muxI1.pin(1), {0x01, Channel_1} },
    { muxI1.pin(2), {0x02, Channel_1} },
    { muxI1.pin(3), {0x03, Channel_1} },
    { muxI1.pin(4), {0x04, Channel_1} },
    { muxI1.pin(5), {0x05, Channel_1} },
    { muxI1.pin(6), {0x06, Channel_1} },
    { muxI1.pin(7), {0x07, Channel_1} },
};
CCPotentiometer I2_POTS[] {
    { muxI2.pin(0), {0x08, Channel_1} },
    { muxI2.pin(1), {0x09, Channel_1} },
    { muxI2.pin(2), {0x0A, Channel_1} },
    { muxI2.pin(3), {0x0B, Channel_1} },
    { muxI2.pin(4), {0x0C, Channel_1} },
    { muxI2.pin(5), {0x0D, Channel_1} },
    { muxI2.pin(6), {0x0E, Channel_1} },
    { muxI2.pin(7), {0x0F, Channel_1} },
};
CCPotentiometer I3_POTS[] {
    { muxI3.pin(0), {0x10, Channel_1} },
    { muxI3.pin(1), {0x11, Channel_1} },
    { muxI3.pin(2), {0x12, Channel_1} },
    { muxI3.pin(3), {0x13, Channel_1} },
    { muxI3.pin(4), {0x14, Channel_1} },
    { muxI3.pin(5), {0x15, Channel_1} },
    { muxI3.pin(6), {0x16, Channel_1} },
    { muxI3.pin(7), {0x17, Channel_1} },
};
CCPotentiometer I4_POTS[] {
    { muxI4.pin(0), {0x18, Channel_1} },
    { muxI4.pin(1), {0x19, Channel_1} },
    { muxI4.pin(2), {0x1A, Channel_1} },
    { muxI4.pin(3), {0x1B, Channel_1} },
    { muxI4.pin(4), {0x1C, Channel_1} },
    { muxI4.pin(5), {0x1D, Channel_1} },
    { muxI4.pin(6), {0x1E, Channel_1} },
    { muxI4.pin(7), {0x1F, Channel_1} },
};
CCPotentiometer I5_POTS[] {
    { muxI5.pin(0), {0x20, Channel_1} },
    { muxI5.pin(1), {0x21, Channel_1} },
    { muxI5.pin(2), {0x22, Channel_1} },
    { muxI5.pin(3), {0x23, Channel_1} },
    { muxI5.pin(4), {0x24, Channel_1} },
    { muxI5.pin(5), {0x25, Channel_1} },
    { muxI5.pin(6), {0x26, Channel_1} },
    { muxI5.pin(7), {0x27, Channel_1} },
};
CCPotentiometer I6_POTS[] {
    { muxI6.pin(0), {0x28, Channel_1} },
    { muxI6.pin(1), {0x29, Channel_1} },
    { muxI6.pin(2), {0x2A, Channel_1} },
    { muxI6.pin(3), {0x2B, Channel_1} },
    { muxI6.pin(4), {0x2C, Channel_1} },
    { muxI6.pin(5), {0x2D, Channel_1} },
    { muxI6.pin(6), {0x2E, Channel_1} },
    { muxI6.pin(7), {0x2F, Channel_1} },
};
// CCPotentiometer I7_POTS[] {
//     { muxI7.pin(0), {0x30, Channel_1} },
//     { muxI7.pin(1), {0x31, Channel_1} },
//     { muxI7.pin(2), {0x32, Channel_1} },
//     { muxI7.pin(3), {0x33, Channel_1} },
//     { muxI7.pin(4), {0x34, Channel_1} },
//     { muxI7.pin(5), {0x35, Channel_1} },
//     { muxI7.pin(6), {0x36, Channel_1} },
//     { muxI7.pin(7), {0x37, Channel_1} },
// };
CCPotentiometer I8_POTS[] {
    { muxI8.pin(0), {0x38, Channel_1} },
    { muxI8.pin(1), {0x39, Channel_1} },
    { muxI8.pin(2), {0x3A, Channel_1} },
    { muxI8.pin(3), {0x3B, Channel_1} },
    { muxI8.pin(4), {0x3C, Channel_1} },
    { muxI8.pin(5), {0x3D, Channel_1} },
    { muxI8.pin(6), {0x3E, Channel_1} },
    { muxI8.pin(7), {0x3F, Channel_1} },
};

//test vars
bool run = 0;
int bpm = 0;

//single button
Button testButton {muxI8.pin(1)};

//button matrix
Button testButtons[] {
  muxI8.pin(1),
  muxI8.pin(2),
  muxI8.pin(3)
  };

//Potentiometer matrix (could be done by Analog or FilteredAnalog but I didn't get it to work)
// so sending to a generic controller on a probably unused channel
CCPotentiometer internalPots[] {
  {muxI8.pin(1), {MIDI_CC::General_Purpose_Controller_1, Channel_14} },
  {muxI8.pin(2), {MIDI_CC::General_Purpose_Controller_1, Channel_14} },
  {muxI8.pin(3), {MIDI_CC::General_Purpose_Controller_1, Channel_14} }
};
//analog matrix
// auto potCollection = copyAs<FilteredAnalog<>>(
//   muxI8.pin(1),
//   muxI8.pin(2),
//   muxI8.pin(3)
//   );

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
  
  updateInternalVariables();
}

//writes the potentiometer pins
void updateInternalVariables() {
  run = testButton.getState();

  run = testButtons[0].getState();

  bpm = internalPots[0].getValue();
  bpm = internalPots[1].getValue();
}
