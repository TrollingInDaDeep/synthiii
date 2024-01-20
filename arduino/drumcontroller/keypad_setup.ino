const byte ROWS = 4;
const byte COLS = 4;

int kpc = 144; // Channel 1 Note on
int midiC = 60; // Standard midi C
int transpose = 0;

//hex because only 1 digit char possible
char hexaKeys[ROWS][COLS] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', 'A', 'B'},
  {'C', 'D', 'E', 'F'}
};
int howManyKeypadKeys = 16;
char drumPadChars [16] = {'1', '2', '3', '+', '4', '5', '6', '>', '7', '8', '9', '!', '*', '0', '#', 'R'};


// flachbandkabel:
// weisser leerer pin links
// R1, R2, C1, C2, C3, C4, R3, R4
byte rowPins[ROWS] = {7, 6, 8, 9}; 
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad kpd = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
