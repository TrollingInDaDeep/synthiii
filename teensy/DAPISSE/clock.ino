// reading the code of clocked vcv rack module
// https://github.com/MarcBoule/ImpromptuModular/blob/master/src/Clocked.cpp#L652
// realizing a separate master clock is a way better idea
// clocked is a great module


///Variables

//bpm
//ticks per beat (tempo)
// number of subclocks
// for each subclock:
    //Ratio (multiplication or division)
    //from ratio calculated ms time when to do next tick
    //delay (1/4 bpm later)
    //connected to which instrument (index)
    // gate time



//Masterclock
//prevStepTime
//nextStepTime


//external clock handler
//tap tempo


// subclocks



// connect subclocks to
 // a -> the sequencer
 // b -> drum instrument 1
 // c -> drum instrument 2

///
/// Functions
///


// updateClock -> run several times also during digital Read
// check if any action to do (tick or subtick)
// update subticks



 // divide / multiply

 //trigger actions for each subclock
 //either seq step
 //or drum instrument trigger
 //or note stop -> maybe should be handled via gate time of each instrument by itself