#include <Keypad.h>

void readDrumpad(bool seq) {
    // supports up to ten simultaneous key presses
      if (kpd.getKeys())  
    {
      // scan the whole key list
      for (int i = 0; i < LIST_MAX; i++) 
      {
        // find keys that have changed state
        if ( kpd.key[i].stateChanged)   
        {
          // when keys are pressed
          if (kpd.key[i].kstate == PRESSED) 
          {
            //sequencer mode, inverse kick note
            if (seq){
              kickNotes[int(kpd.key[i].kchar)] = !kickNotes[int(kpd.key[i].kchar)];
            }
            //play mode, start note
            else {
              startNote(midiC + transpose + int(kpd.key[i].kchar));
            }
          }


          // when keys are released
          if (kpd.key[i].kstate == RELEASED)  
          {
            //seq mode, do nothing
            if (seq){
            }
            //play mode, stop note
            else {
              stopNote(midiC + transpose + int(kpd.key[i].kchar));
            }
          }
            
        }
      }
    }
}
    //pressed:
    
            // switch (kpd.key[i].kchar) {
            //   case '1':
            //         if (seq) {kickNotes[0] =! kickNotes[0];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 0);
            //         }
            //         break;

            //       case '2':
            //         if (seq) {kickNotes[1] =! kickNotes[1];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 1);
            //         }
            //         break;

            //       case '3':
            //         if (seq) {kickNotes[2] =! kickNotes[2];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 2);
            //         }
            //         break;

            //       case '+':
            //         if (seq) {kickNotes[3] =! kickNotes[3];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 3);
            //         }
            //         break;

            //       case '4':
            //         if (seq) {kickNotes[4] =! kickNotes[4];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 4);
            //         }
            //         break;

            //       case '5':
            //         if (seq) {kickNotes[5] =! kickNotes[5];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 5);
            //         }
            //         break;

            //       case '6':
            //         if (seq) {kickNotes[6] =! kickNotes[6];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 6);
            //         }
            //         break;

            //       case '>':
            //         if (seq) {kickNotes[7] =! kickNotes[7];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 7);
            //         }
            //         break;

            //       case '7':
            //         if (seq) {kickNotes[8] =! kickNotes[8];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 8);
            //         }
            //         break;

            //       case '8':
            //         if (seq) {kickNotes[9] =! kickNotes[9];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 9);
            //         }
            //         break;

            //       case '9':
            //         if (seq) {kickNotes[10] =! kickNotes[10];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 10);
            //         }
            //         break;

            //       case '!':
            //         if (seq) {kickNotes[11] =! kickNotes[11];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 11);
            //         }
            //         break;

            //       case '*':
            //         if (seq) {kickNotes[12] =! kickNotes[12];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 12);
            //         }
            //         break;

            //       case '0':
            //         if (seq) {kickNotes[13] =! kickNotes[13];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 13);
            //         }
            //         break;

            //       case '#':
            //         if (seq) {kickNotes[14] =! kickNotes[14];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 14);
            //         }
            //         break;

            //       case 'R':
            //         if (seq) {kickNotes[15] =! kickNotes[15];} // inverse kickNote
            //         else {
            //         startNote(midiC + transpose + 15);
            //         }
            //         break;

            // }


    // released
    // switch (kpd.key[i].kchar) {
    //           case '1':
    //                 stopNote(midiC + transpose + 0);
    //                 break;
    //               case '2':
    //                 stopNote(midiC + transpose + 1);
    //                 break;
    //               case '3':
    //                 stopNote(midiC + transpose + 2);
    //                 break;
    //               case '+':
    //                 stopNote(midiC + transpose + 3);
    //                 break;
    //               case '4':
    //                 stopNote(midiC + transpose + 4);
    //                 break;
    //               case '5':
    //                 stopNote(midiC + transpose + 5);
    //                 break;
    //               case '6':
    //                 stopNote(midiC + transpose + 6);
    //                 break;
    //               case '>':
    //                 stopNote(midiC + transpose + 7);
    //                 break;
    //               case '7':
    //                 stopNote(midiC + transpose + 8);
    //                 break;
    //               case '8':
    //                 stopNote(midiC + transpose + 9);
    //                 break;
    //               case '9':
    //                 stopNote(midiC + transpose + 10);
    //                 break;
    //               case '!':
    //                 stopNote(midiC + transpose + 11);
    //                 break;
    //               case '*':
    //                 stopNote(midiC + transpose + 12);
    //                 break;
    //               case '0':
    //                 stopNote(midiC + transpose + 13);
    //                 break;
    //               case '#':
    //                 stopNote(midiC + transpose + 14);
    //                 break;
    //               case 'R':
    //                 stopNote(midiC + transpose + 15);
    //                 break;

    //         }
    //       }
    //     }

    
