// code stolen from https://chariscat.wordpress.com/2020/12/01/adding-additional-analog-inputs-in-arduino-cd4051be-multiplexing-chip/
// modified a bit
// 2 CD4051B with potentiometers connected to them
// inputs (the common i/o pin of every chip, pin 3 on the CD4051B) go to A4 and A5 on arduino
// +5v arduino is connected to Vdd (pin 16)
// GND of arduino is connected to vss (pin 8)
// Selector Pins A,B,C are connected to digital pins 0,1,2 on the arduino
// so from each mux chip:
//pin 11/A goes to digitalpin 8 on arduino
//pin 12/B goes to digitalpin 9 on arduino
//pin 13/C goes to digitalpin 10 on arduino
// inbit (pin 6 on mux goes to digipin 3 on arduino
int inbit=3;
int wait= 10;

int analogIn_1 = 5;
int analogIn_2 = 4;

 void setup() 
 {
   Serial.begin(9600);
   pinMode(inbit,OUTPUT); //Inhibit set to off
   digitalWrite(inbit,LOW);
   DDRB = 0b00000111;  //PORT 8,9,10 as output
   PORTB = 0b00000000; //PORT 8,9,10 set to LOW
   pinMode(analogIn_1,INPUT);
   pinMode(analogIn_2,INPUT);
 }
 void loop() 
 {
   PORTB=0b00000000; //Data flow - X0
   int Sensor0 =  analogRead(analogIn_1);//read data from X0
   Serial.print("Sensor0:");
   Serial.println(Sensor0);
   delay(wait);
 PORTB=0b00000001;  //Data flow - X1
   int Sensor1 =  analogRead(analogIn_1); //read data from X1
   Serial.print("Sensor1:");
   Serial.println(Sensor1);
   delay(wait);
 PORTB=0b00000010;  //Data flow - X2
   int Sensor2 =  analogRead(analogIn_1); //read data from X2
   Serial.print("Sensor2:");
   Serial.println(Sensor2);
   delay(wait);
 PORTB=0b00000011;  //Data flow - X3
   int Sensor3 =  analogRead(analogIn_1); //read data from X3
   Serial.print("Sensor3:");
   Serial.println(Sensor3);
   delay(wait);
 PORTB=0b00000100;  //Data flow - X4
   int Sensor4 =  analogRead(analogIn_1); //read data from X4
   Serial.print("Sensor4:");
   Serial.println(Sensor4);
   delay(wait);
 PORTB=0b00000101;  //Data flow - X5
   int Sensor5 =  analogRead(analogIn_1); //read data from X5
   Serial.print("Sensor5:");
   Serial.println(Sensor5);
   delay(wait);
 PORTB=0b00000110;  //Data flow - X6
   int Sensor6 =  analogRead(analogIn_1); //read data from X6
   Serial.print("Sensor6:");
   Serial.println(Sensor6);
   delay(wait);
 PORTB=0b00000111;  //Data flow - X7
   int Sensor7 =  analogRead(analogIn_1); //read data from X7
   Serial.print("Sensor7:");
   Serial.println(Sensor7);
   delay(wait);
 digitalWrite(inbit,HIGH);  // Inhibit / stop all data
   delay(wait);  
   digitalWrite(inbit,LOW);  // Inhibit /  let data flow
 }
