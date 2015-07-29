
// openknit control
// Taylor Hokanson and Dieter Kirkwood
// based on the work of TBA

// Button: GND > Momentary > Pin 12
// Potentiometer: A0 > Pot center pin, other two pins go to 5V and GND
// Encoder:

// Button
const int buttonPin = 12;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

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

  // Button
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  resetBEDPins();           // Set step, direction, microstep and enable pins to default states
  digitalWrite(dir, HIGH);  // Direction = Forward/CCW
  digitalWrite(EN, LOW);    // Enable = True

  //Serial.begin(9600);
}

//Main loop
void loop() { 
  checkButton();
  //checkPot();
  //checkEncoder();
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
/*
void checkEncoder(){
  newKnobPos = eKnob.read();
  if (newKnobPos != oldKnobPos) {
    Serial.print("Encoder = ");
    Serial.print(newKnobPos);
    Serial.println();
    oldKnobPos = newKnobPos;
  }
}

void checkPot(){
  if(newPotVal > oldPotVal + 3 || newPotVal < oldPotVal - 3){
    Serial.println("Pot value: " + String(newPotVal) + "\n");
    oldPotVal = newPotVal;
  }
}
*/
void checkButton(){
int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }
  digitalWrite(ledPin, ledState);
  digitalWrite(dir, ledState);
  lastButtonState = reading;
}

void takeStep(){
  
  int stepDelay = 8;
  for(int j= 0; j<5; j++){    //Loop the forward stepping enough times for motion to be visible
    digitalWrite(stp,HIGH);   //Trigger one step forward
    delay(stepDelay);
    digitalWrite(stp,LOW);    //Pull step pin low so it can be triggered again
    delay(stepDelay);
  }   
}

