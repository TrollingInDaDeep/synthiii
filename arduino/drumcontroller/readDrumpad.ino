#include <Keypad.h>

void readDrumpad() {
    // supports up to ten simultaneous key presses
      if (kpd.getKeys())  
    {
      // scan the whole key list
      for (int i = 0; i < howManyKeypadKeys; i++) 
      {
        // Seht selbst
        int keyNumber = youFuckenKiddingMe(kpd.key[i].kchar);

        // find keys that have changed state
        if ( kpd.key[i].stateChanged)   
        {
          // when keys are pressed
          if (kpd.key[i].kstate == PRESSED) 
          {
            switch (keypadMode) {
              //Play Mode
              case 0:
                startNote(midiC + transpose + keyNumber);
              break;

              //enable / disable notes sequence Mode
              case 1:
                drumSequence[selectedInstrument][keyNumber] = !drumSequence[selectedInstrument][keyNumber];
              break;

              // Fill every x step Mode
              case 2:

              break;

              //Settings mode
              case 3:

              break;
            }
          }


          // when keys are released
          if (kpd.key[i].kstate == RELEASED)  
          {

            switch (keypadMode) {
              //Play Mode
              case 0:
                stopNote(midiC + transpose + keyNumber);
              break;

              //enable / disable notes sequence Mode
              case 1:

              break;

              // Fill every x step Mode
              case 2:

              break;

              //Settings mode
              case 3:

              break;
            }
            
          }
            
        }
      }
    }
}

    
