











 //clockHandler switchcase for drums

    case 1: // kick
      if (subClocks[subClockID][8] == 1){ // if subclock is running
        if (subClocks[subClockID][9] == 1){ //note Start
          startDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
          //Serial.print("pulse Kick @");
          //Serial.println(currentTick);
          
          subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
          subClocks[subClockID][11] = currentMillis;
        }
        else { // note Stop
          if (subClocks[subClockID][10] == 0) { //if note not yet stopped
            stopDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
          }
          subClocks[subClockID][9] = 1; //set note to START mode
        }
      }
    break;

    case 2: //snare
      if (subClocks[subClockID][8] == 1){ // if subclock is running
        if (subClocks[subClockID][9] == 1){ //note Start
          startDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],1); //take the instrument note from field 6 (instrument) from subClocks Array
          //Serial.print("pulse Kick @");
          //Serial.println(currentTick);
          
          subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
          subClocks[subClockID][11] = currentMillis;
        }
        else { // note Stop
          if (subClocks[subClockID][10] == 0) { //if note not yet stopped
            stopDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],1); //take the instrument note from field 6 (instrument) from subClocks Array
          }
          subClocks[subClockID][9] = 1; //set note to START mode
        }
      }
    break;

    case 3: //hat
      if (subClocks[subClockID][8] == 1){ // if subclock is running
        if (subClocks[subClockID][9] == 1){ //note Start
          startDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],2); //take the instrument note from field 6 (instrument) from subClocks Array
          //Serial.print("pulse Kick @");
          //Serial.println(currentTick);
          
          subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
          subClocks[subClockID][11] = currentMillis;
        }
        else { // note Stop
          if (subClocks[subClockID][10] == 0) { //if note not yet stopped
            stopDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],2); //take the instrument note from field 6 (instrument) from subClocks Array
          }
          subClocks[subClockID][9] = 1; //set note to START mode
        }
      }
    break;

    case 4: //cymbal
      if (subClocks[subClockID][8] == 1){ // if subclock is running
        if (subClocks[subClockID][9] == 1){ //note Start
          startDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],3); //take the instrument note from field 6 (instrument) from subClocks Array
          //Serial.print("pulse Kick @");
          //Serial.println(currentTick);
          
          subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
          subClocks[subClockID][11] = currentMillis;
        }
        else { // note Stop
          if (subClocks[subClockID][10] == 0) { //if note not yet stopped
            stopDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],3); //take the instrument note from field 6 (instrument) from subClocks Array
          }
          subClocks[subClockID][9] = 1; //set note to START mode
        }
      }
    break;

    // case 2: // snare
    //   if (subClocks[subClockID][8] == 1){ // if subclock is running
    //     if (subClocks[subClockID][9] == 1){ //note Start
    //       startDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
    //       //Serial.print("pulse snare @");
    //       //Serial.println(currentTick);
          
    //       subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
    //       subClocks[subClockID][11] = currentMillis;
    //     }
    //     else { // note Stop
    //       if (!drumNoteStopped[0]) { //snare
    //         stopDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
    //       }
    //       subClocks[subClockID][9] = 1; //set note to START mode
    //     }
    //   }
    // break;

    // case 3: // hat
    //   if (subClocks[subClockID][8] == 1){ // if subclock is running
    //     if (subClocks[subClockID][9] == 1){ //note Start
    //       startDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
    //       //Serial.print("pulse snare @");
    //       //Serial.println(currentTick);
          
    //       subClocks[subClockID][10] = 0; //tell that stop hasn't been sent yet
    //       subClocks[subClockID][11] = currentMillis;
    //     }
    //     else { // note Stop
    //       if (!drumNoteStopped[0]) { //snare
    //         stopDrumNote(instrumentNotes[1][int(subClocks[subClockID][6])],0); //take the instrument note from field 6 (instrument) from subClocks Array
    //       }
    //       subClocks[subClockID][9] = 1; //set note to START mode
    //     }
    //   }
    // break;

