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

void setup() {
  
  // Button
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

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
}

// Pot
ISR(ADC_vect) {             //when new ADC value ready
  newPotVal = ADCH;         //update the variable newPotVal with new value from A0 (between 0 and 255)
}

void loop() {
  checkButton();
  checkPot();
  checkEncoder();
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
