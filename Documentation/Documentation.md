# Documentation
With the "Dapisse" I built a Midi controller (turning knobs, controlling software synths), which also acts as a midi sequencer. The sequencer is based on the intellijel metropolis, an awesome sequencer, shown by mylarmelodies here: https://www.youtube.com/watch?v=uV9-XA5MPwY

also included, we have a drumpad that can play midi notes to trigger drum sounds, but it can also create rhythms and different drum beats in changing intensity.

## Function diagram
A visual representation of how the Controller works
![functional diagram](./synthii_functional_diagram.png)

## Images

## What the controls do

### Midi Controller

### Sequencer

### Drumpad
Drumpad or Drum Machine will send Midi notes for example to trigger a drum sample. There are 4 "Instruments", just means 4 different Midi notes that will be triggered.
By default an Instrumen means a Kick, Snare, Hat and Cymbal. The Drum Machine also runs off the same Steps as the Sequencer, so that it is in sync. That's why I called it Instruments, The Synth is an instrument, the Kick is one etc... 
Default Midi Notes, defined in array drumInstrumentNotes[].
63, //Kick
67, //snare
71, //highhat
75  //cymbals

Each instrument is triggered from a different SubClock. Instruments 0-9 are Drum instruments, higher are "normal" instruments. Difference: Drum instruments only send NoteOn as trigger, no noteOff and not Gate Time.

#### Keypad Modes
Via a Pot you can select between 4 Keypad Modes
1: Play
In this mode, the Keypad is just a midi notePad or Keyboard. It will play notes from 60 to 75 on the drum Channel. The rightmost column corresponds the midi notes of the 4 Instruments (kick, snare, hat, cymbal). You can simply play notes, also the drum notes. In the loop, the drumpad will not be read, hence no else drumpad function is executed.
Keys are initialized as "NoteButtonMatrix<4, 4> keypadMatrix". It will send the midi notes always, i didn't get to fix this yet. midi notes should only be sent in play mode. in other modes, the midi notes should be sent via the sequencer. i tried running keypadmatrix.enable() and .disable(), but that results in no control updating whatsoever.
Workaround would be to use a different midi channel for the keypadMatrix and the sequenced drum midi notes. maybe also use a filter within this code to filter out the keypad notes conditionally.

2: Perform
not implemented yet. idea is to do like a po-33 with ratchets, live effects on an existing drum beat.

3: setRate

4: I need a Drummer

## Code

## Sound examples
