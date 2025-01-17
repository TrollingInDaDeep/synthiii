#include <Control_Surface.h>

// Instantiate a MIDI Interface to use
USBMIDI_Interface midi;
FilteredAnalog<10,3,uint32_t> testPot = A10;
/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A10, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(A17, INPUT);
  pinMode(A0, INPUT);
  pinMode(A4, INPUT);
  pinMode(A9, INPUT);
  
  Serial.begin(9600);
  Control_Surface.begin();

  FilteredAnalog<>::setupADC();

}

// the loop function runs over and over again forever
void loop() {
  Control_Surface.loop();
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  //delay(200);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  //delay(1000);                      // wait for a second
  //Serial.println(analogRead(A10));
  testPot.update();
  Serial.println(testPot.getValue());
  //Serial.println(analogRead(A13));
  //Serial.println(analogRead(A14));
  //Serial.println(analogRead(A17));
  //Serial.println(analogRead(A4));
  //Serial.println(analogRead(A9));

}
