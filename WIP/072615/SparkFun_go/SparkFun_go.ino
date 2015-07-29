/****************************************************************************** 
SparkFun Big Easy Driver Basic Demo
Toni Klopfenstein @ SparkFun Electronics
February 2015
https://github.com/sparkfun/Big_Easy_Driver

Simple demo sketch to demonstrate how 5 digital pins can drive a bipolar stepper motor,
using the Big Easy Driver (https://www.sparkfun.com/products/12859). Also shows the ability to change
microstep size, and direction of motor movement.

Development environment specifics:
Written in Arduino 1.6.0

This code is beerware; if you see me (or any other SparkFun employee) at the local, and you've found our code helpful, please buy us a round!
Distributed as-is; no warranty is given.

Example based off of demos by Brian Schmalz (designer of the Big Easy Driver).
http://www.schmalzhaus.com/EasyDriver/Examples/EasyDriverExamples.html
******************************************************************************/
//Declare pin functions on Arduino
#define stp 7
#define dir 6
#define MS1 10
#define MS2 9
#define MS3 8
#define EN  11

//Declare variables for functions
char user_input;
int x;
int y;
int state;

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
  go();
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

void go(){

/*  digitalWrite(stp,HIGH); //Trigger one step forward
  delay(4);
  digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
  delay(4);
*/  
  
  int stepDelay = 8;
  
  for(x= 1; x<1000; x++){  //Loop the forward stepping enough times for motion to be visible
    for(int j= 0; j<20; j++){  //Loop the forward stepping enough times for motion to be visible
      digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"
      digitalWrite(stp,HIGH); //Trigger one step forward
      delay(stepDelay);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(stepDelay);
    }
/*
      for(int y= 0; y<15; y++){  //Loop the forward stepping enough times for motion to be visible
      digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
      digitalWrite(stp,HIGH); //Trigger one step forward
      delay(stepDelay);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(stepDelay);
    }  
*/    
  }
}

//Default microstep mode function
void StepForwardDefault(){
  for(x= 1; x<1000; x++){  //Loop the forward stepping enough times for motion to be visible
    for(int j= 0; j<20; j++){  //Loop the forward stepping enough times for motion to be visible
      digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
      digitalWrite(stp,HIGH); //Trigger one step forward
      delay(1);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(1);
    }

      for(int y= 0; y<5; y++){  //Loop the forward stepping enough times for motion to be visible
      digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"
      digitalWrite(stp,HIGH); //Trigger one step forward
      delay(1);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(1);
    }    
  }
}

//Reverse default microstep mode function
void ReverseStepDefault()
{
  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 1; x<1000; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}

// 1/16th microstep foward mode function
void SmallStepMode()
{
  Serial.println("Stepping at 1/16th microstep mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  digitalWrite(MS1, HIGH); //Pull MS1,MS2, and MS3 high to set logic to 1/16th microstep resolution
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}

//Forward/reverse stepping function
void ForwardBackwardStep()
{
  for(x= 1; x<5; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    //Read direction pin state and change it
    state=digitalRead(dir);
    if(state == HIGH)
    {
      digitalWrite(dir, LOW);
    }
    else if(state ==LOW)
    {
      digitalWrite(dir,HIGH);
    }
    
    for(y=1; y<1000; y++)
    {
      digitalWrite(stp,HIGH); //Trigger one step
      delay(1);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(1);
    }
  }
}
