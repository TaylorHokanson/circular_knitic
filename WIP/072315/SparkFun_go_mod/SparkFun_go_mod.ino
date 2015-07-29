#include <Encoder.h>
Encoder eKnob(2, 3);

//Declare pin functions on Arduino
#define stp 8
#define dir 9
#define MS1 4
#define MS2 5
#define MS3 6
#define EN  7

//Declare variables for functions
char user_input;
int x;
int y;
int state;

long oldKnobPos  = -999;
float oldPotValue = 1000;
long newKnobPos = 0;
float newPotValue = 0;
int motorSpeed = 0;

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  resetBEDPins(); //Set step, direction, microstep and enable pins to default states
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
}

//Main loop
void loop() {
  digitalWrite(EN, LOW); //Pull enable pin low to set FETs active and allow motor control
  digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"  
  checkKnobs();
  go();
}

void checkKnobs(){
  newKnobPos = eKnob.read();
  newPotValue = analogRead(A0);
  motorSpeed = newPotValue * (5.0 / 1023.0);
}

void go(){

  if (motorSpeed > 2.5){
    digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(motorSpeed * 2);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(motorSpeed * 2);   
  }
}

//Reset Big Easy Driver pins to default states
void resetBEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(EN, HIGH);
}
