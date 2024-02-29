#include <Control_Surface.h>
#include <MIDIUSB.h>
USBMIDI_Interface midi;

// PINS
const int 


//Multiplexers
CD74HC4051 I1 {38, {37, 38, 39} };
CD74HC4051 I2 {41, {37, 38, 39} };
CD74HC4051 I3 {40, {37, 38, 39} };
CD74HC4051 I4 {39, {37, 38, 39} };
CD74HC4051 I5 {25, {37, 38, 39} };
CD74HC4051 I6 {26, {37, 38, 39} };
CD74HC4051 I7 {11, {37, 38, 39} };
CD74HC4051 I8 {12, {37, 38, 39} };


// Controls
//CCPotentiometer
CCPotentiometer I1_POTS[] {
    { I1.pin(0), {0x00, Channel_1} },
    { I1.pin(1), {0x01, Channel_2} },
    { I1.pin(2), {0x02, Channel_3} },
    { I1.pin(3), {0x03, Channel_4} },
    { I1.pin(4), {0x04, Channel_5} },
    { I1.pin(5), {0x05, Channel_6} },
    { I1.pin(6), {0x06, Channel_7} },
    { I1.pin(7), {0x07, Channel_8} },
};
CCPotentiometer I2_POTS[] {
    { I2.pin(0), {0x08, Channel_9} },
    { I2.pin(1), {0x09, Channel_10} },
    { I2.pin(2), {0x0A, Channel_11} },
    { I2.pin(3), {0x0B, Channel_12} },
    { I2.pin(4), {0x0C, Channel_13} },
    { I2.pin(5), {0x0D, Channel_14} },
    { I2.pin(6), {0x0E, Channel_15} },
    { I2.pin(7), {0x0F, Channel_16} },
};
CCPotentiometer I3_POTS[] {
    { I3.pin(0), {0x10, Channel_1} },
    { I3.pin(1), {0x11, Channel_2} },
    { I3.pin(2), {0x12, Channel_3} },
    { I3.pin(3), {0x13, Channel_4} },
    { I3.pin(4), {0x14, Channel_5} },
    { I3.pin(5), {0x15, Channel_6} },
    { I3.pin(6), {0x16, Channel_7} },
    { I3.pin(7), {0x17, Channel_8} },
};
CCPotentiometer I4_POTS[] {
    { I4.pin(0), {0x18, Channel_9} },
    { I4.pin(1), {0x19, Channel_10} },
    { I4.pin(2), {0x1A, Channel_11} },
    { I4.pin(3), {0x1B, Channel_12} },
    { I4.pin(4), {0x1C, Channel_13} },
    { I4.pin(5), {0x1D, Channel_14} },
    { I4.pin(6), {0x1E, Channel_15} },
    { I4.pin(7), {0x1F, Channel_16} },
};
CCPotentiometer I5_POTS[] {
    { I5.pin(0), {0x20, Channel_1} },
    { I5.pin(1), {0x21, Channel_2} },
    { I5.pin(2), {0x22, Channel_3} },
    { I5.pin(3), {0x23, Channel_4} },
    { I5.pin(4), {0x24, Channel_5} },
    { I5.pin(5), {0x25, Channel_6} },
    { I5.pin(6), {0x26, Channel_7} },
    { I5.pin(7), {0x27, Channel_8} },
};
CCPotentiometer I6_POTS[] {
    { I6.pin(0), {0x28, Channel_9} },
    { I6.pin(1), {0x29, Channel_10} },
    { I6.pin(2), {0x2A, Channel_11} },
    { I6.pin(3), {0x2B, Channel_12} },
    { I6.pin(4), {0x2C, Channel_13} },
    { I6.pin(5), {0x2D, Channel_14} },
    { I6.pin(6), {0x2E, Channel_15} },
    { I6.pin(7), {0x2F, Channel_16} },
};
CCPotentiometer I7_POTS[] {
    { I7.pin(0), {0x30, Channel_1} },
    { I7.pin(1), {0x31, Channel_2} },
    { I7.pin(2), {0x32, Channel_3} },
    { I7.pin(3), {0x33, Channel_4} },
    { I7.pin(4), {0x34, Channel_5} },
    { I7.pin(5), {0x35, Channel_6} },
    { I7.pin(6), {0x36, Channel_7} },
    { I7.pin(7), {0x37, Channel_8} },
};
CCPotentiometer I8_POTS[] {
    { I8.pin(0), {0x38, Channel_9} },
    { I8.pin(1), {0x39, Channel_10} },
    { I8.pin(2), {0x3A, Channel_11} },
    { I8.pin(3), {0x3B, Channel_12} },
    { I8.pin(4), {0x3C, Channel_13} },
    { I8.pin(5), {0x3D, Channel_14} },
    { I8.pin(6), {0x3E, Channel_15} },
    { I8.pin(7), {0x3F, Channel_16} },
};
void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();

}
