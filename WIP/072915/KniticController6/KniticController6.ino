// direction button and speed pot working, encoder not working

// openknit control
// Taylor Hokanson and Dieter Kirkwood
// based on the work of TBA

// Button: GND > Momentary > Pin 12
// Potentiometer: A0 > Pot center pin, other two pins go to 5V and GND
// Encoder:

// Encoder
#include <Encoder.h>
Encoder eKnob(2, 3);
long oldKnobPos  = -999;
long newKnobPos = 0;

// Pot
int newPotVal;
int oldPotVal;
int motorSpeed = 0;
int motorJam = 0;

// Button
const int buttonPin = 12;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin
int ledState = LOW;          // start button state LOW (motor forward)
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

  // Pot
  ADCSRA = 0;
  ADCSRB = 0;
  ADMUX |= (1 << REFS0);                    //set reference voltage
  ADMUX |= (1 << ADLAR);                    //left align the ADC value- so we can read highest 8 bits from ADCH register only
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE);                   //enabble auto trigger
  ADCSRA |= (1 << ADIE);                    //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN);                    //enable ADC
  ADCSRA |= (1 << ADSC);                    //start ADC measurements
  sei();                                    //enable interrupts

  Serial.begin(9600);
  takeStep();
}

// Pot
ISR(ADC_vect) {             //when new ADC value ready
  newPotVal = ADCH;         //update the variable newPotVal with new value from A0 (between 0 and 255)
}

//Main loop
void loop() { 
  checkButton();
  checkPot();
  takeStep();
  checkEncoder();
}

//Reset Big Easy Driver pins to default states
void resetBEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, HIGH);  // HIGH = Forward/CCW
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(EN, LOW);    // LOW = Enable Motor Control
}

void checkEncoder(){
  
  newKnobPos = eKnob.read();
  if (newKnobPos != oldKnobPos) {
    digitalWrite(EN, LOW);    // enable Motor Control
    Serial.print("Encoder = ");
    Serial.print(newKnobPos);
    Serial.println();
    oldKnobPos = newKnobPos;
  }else{
    Serial.println("Motor Stopped!\n");
    //motorJam++;
   digitalWrite(EN, HIGH);    // LOW = Disable Motor Control
  }
/*  if (motorJam > 10){
    digitalWrite(EN, HIGH);      // LOW = Disable Motor Control
  }*/
}

void checkPot(){
  if(newPotVal > oldPotVal + 3 || newPotVal < oldPotVal - 3){
    motorSpeed = map(newPotVal, 0, 254, 0, 10); 
    Serial.println("Pot value: " + String(motorSpeed) + "\n");  
    oldPotVal = newPotVal;
  }
}

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
  int stepDelay = motorSpeed;
//  int checkMotor = eKnob.read();
  for(int j= 0; j<5; j++){    //Loop the forward stepping enough times for motion to be visible
    digitalWrite(stp,HIGH);   //Trigger one step forward
    delay(stepDelay);
    digitalWrite(stp,LOW);    //Pull step pin low so it can be triggered again
    delay(stepDelay);
  }
}

