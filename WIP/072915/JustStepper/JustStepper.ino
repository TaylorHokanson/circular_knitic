
//Declare pin functions on Arduino
#define stp 7
#define dir 6
#define MS1 10
#define MS2 9
#define MS3 8
#define EN  11

/*
//Declare variables for functions
char user_input;
int x;
int y;
int state;
*/

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  resetBEDPins(); //Set step, direction, microstep and enable pins to default states
  digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"
  digitalWrite(EN, LOW); //Pull enable pin low to set FETs active and allow motor control
}

//Main loop
void loop() { 
  takeStep();
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

void takeStep(){
  //digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"  
  int stepDelay = 8;
  for(int j= 0; j<5; j++){    //Loop the forward stepping enough times for motion to be visible
    digitalWrite(dir, HIGH);  //Pull direction pin low to move "forward"
    digitalWrite(stp,HIGH);   //Trigger one step forward
    delay(stepDelay);
    digitalWrite(stp,LOW);    //Pull step pin low so it can be triggered again
    delay(stepDelay);
  }   
}

