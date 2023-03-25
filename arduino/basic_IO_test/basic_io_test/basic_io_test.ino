// Analog Pins
const int Ain = 1;

// Ditital Pins
const int Din = 2;
//const int Cin = 0;$$ pinmode setzen
//const int Cout = 2;pinmode setzen
const int arrCinPins[] = {0, 13};
const int arrCoutPins[] = {2, 12};

// Status Bytes
// see links above for documentation
const int statusByteAin = 176; // Status byte for analog inputs. 176 = controlChange Channel 1
const int statusByteDin = 177; // Status byte for analog inputs. 177 = controlChange Channel 2

// Number of Connection inputs and outputs
const int nrCout = 2;  //number of connection outputs
const int nrCin = 2;   // number of connection inputs


// Array that holds all connection states
// first value is Cin index , 2nd value is Cout index
bool arrConnectionStates[nrCin][nrCout];

// var declarations to use later
// stores current state/value of inputs/outputs/mux control pins
int intCurrAin;
bool blnCurrDin;
bool blnCurrCin;
bool blnCurrCout;



int intInputNr; // which input are we on
int intOutputNr; // which output are we on

void setup() {
  //set pin modes
  //Analog
  pinMode(A1, INPUT);  //Ain

  //Digital
  pinMode(Din,INPUT_PULLUP);
  //pinMode(Cin,INPUT_PULLUP);
  pinMode(0,INPUT_PULLUP); //temp
  pinMode(13,INPUT_PULLUP); //temp
  //pinMode(Cout,OUTPUT);
  pinMode(2,OUTPUT); //temp
  pinMode(12,OUTPUT); //temp

  // set outputs to low (inverted logic)
  for (int o=0;o<nrCout;o++) {
    //set Cout pin low (inverted logic)
    digitalWrite(arrCoutPins[o], LOW);
  }

  // set all array vars to 1, fo
  for(int i=0;i<nrCout;i++)
      for(int j=0;j<nrCin;j++)
          arrConnectionStates[i][j] = 1;

  Serial.begin(115200);
  delay(10);
  Serial.println("Initialized");


}

void loop() {

  blnCurrDin = digitalRead(Din);
  //Serial.println(blnCurrDin);
  intCurrAin = analogRead(Ain);
  //Serial.println(intCurrAin);


  for (int o=0;o<nrCout;o++) {

    for(int i=0;i<nrCin;i++){
      blnCurrCin = digitalRead(arrCinPins[i]);

      //fuuck, connects and disconnects both outputs
      //updateConnection(i, o, !blnCurrCin); //send negated value because of inverted logic
      //Serial.print(arrCinPins[i]);
      //Serial.print(":");
      //Serial.print(o);
      //Serial.print(":");
      //Serial.println(blnCurrCin);
      //Serial.println(";");
    }
  }
    

  // Serial.println("$");
}