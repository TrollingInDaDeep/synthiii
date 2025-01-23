
//sets the function of the Sequencer trigger buttons
void selectSeqNoteFunction(){
  if (Metropolis[0].fuSel0 && Metropolis[0].fuSel1){
    //Play Mode
    Metropolis[0].buttonFunction = 0;
  }
  if (!Metropolis[0].fuSel0 && Metropolis[0].fuSel1){
    //Skip mode
    Metropolis[0].buttonFunction = 1;
  }
  if (Metropolis[0].fuSel0 && !Metropolis[0].fuSel1){
    //Slide mode
    Metropolis[0].buttonFunction = 2;
  }
  if (!Metropolis[0].fuSel0 && !Metropolis[0].fuSel1){
    //Hold mode --> idea: play between pressed buttons mode
    Metropolis[0].buttonFunction = 3;
  }
}
