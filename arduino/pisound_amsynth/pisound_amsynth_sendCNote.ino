#include <Control_Surface.h>

USBDebugMIDI_Interface midebug {115200};
HairlessMIDI_Interface mihairless;
USBMIDI_Interface misa;

//vars
const MIDIAddress note = MIDI_Notes::C(4);
const uint8_t velocity = 127;




void setup() {
  mihairless.begin();
}

void loop() {
  mihairless.sendNoteOn(note, velocity);
  delay(500);
  mihairless.update();


  
}
