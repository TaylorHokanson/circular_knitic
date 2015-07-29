// Circular Knictic motor controller
// Taylor Hokanson and Dieter Kirkwood
// we stand on the shoulders of giants:
// https://github.com/sparkfun/Big_Easy_Driver

// Button: GND > Momentary > Pin 12
// Potentiometer: A0 > Pot center pin, other two pins go to 5V and GND
// Rotary Encoder:

// Big Easy
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define MS3 6
#define EN  7

char user_input;
int x;
int y;
int state;

// Encoder
#include <Encoder.h>
Encoder eKnob(2, 3);
long oldKnobPos  = -999;
long newKnobPos = 0;

// Button
const int buttonPin = 12;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Pot
int newPotVal;
int oldPotVal;
int stepDelay;

void setup() {
  
  // Button
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  // Encoder
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

  // Big easy
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  resetBEDPins();         //Set step, direction, microstep and enable pins to default states

  Serial.begin(9600);
}

// Pot
ISR(ADC_vect) {             //when new ADC value ready
  newPotVal = ADCH;         //update the variable newPotVal with new value from A0 (between 0 and 255)
}

void loop() {
  checkButton();
  checkPot();
  checkEncoder();
  go();
}

void go(){
  if (ledState == HIGH){
    digitalWrite(dir, LOW);
  }else{
    digitalWrite(dir, HIGH);
  }
  digitalWrite(dir, HIGH); 
  digitalWrite(stp,HIGH); 
  delay(stepDelay);
  digitalWrite(stp,LOW);
  delay(stepDelay);
}

void resetBEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(EN, HIGH);     // disable motor control
}

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
    stepDelay = newPotVal * (5.0 / 1023.0); 
    Serial.println("Pot value: " + String(newPotVal) + "\n");
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
  lastButtonState = reading;
}
