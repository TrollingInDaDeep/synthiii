///starts a drumnote reading note from drumInstrument array
void startDrumNote(int subClockID) {

  usbMIDI.sendNoteOn(drumInstrumentNotes[subClocks[subClockID].instrument], 127, subClocks[subClockID].midiChannel);
}

///stops a drumnote reading note from drumInstrument array
void stopDrumNote(int subClockID) {
  usbMIDI.sendNoteOff(drumInstrumentNotes[subClocks[subClockID].instrument], 127, subClocks[subClockID].midiChannel);
}