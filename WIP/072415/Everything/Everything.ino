//Circular Knitic with potentiometer speed control, pushbutton direction revesal and rotary encoder feedback
//based on the work of Amanda Ghassaei

int newPotVal;
int oldPotVal;

#include <Encoder.h>
Encoder eKnob(2, 3);
long oldKnobPos  = -999;
long newKnobPos = 0;

void setup(){

  cli();//disable interrupts
  
  //set up continuous sampling of analog pin 0
  
  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  
  ADMUX |= (1 << REFS0);                    //set reference voltage
  ADMUX |= (1 << ADLAR);                    //left align the ADC value- so we can read highest 8 bits from ADCH register only
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE);                   //enabble auto trigger
  ADCSRA |= (1 << ADIE);                    //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN);                    //enable ADC
  ADCSRA |= (1 << ADSC);                    //start ADC measurements
  
  sei();     //enable interrupts

  //if you want to add other things to setup(), do it here
  Serial.begin(9600);
  pinMode(12, INPUT);           // set pin to input
  digitalWrite(12, HIGH);       // turn on pullup resistors
}

ISR(ADC_vect) {             //when new ADC value ready
  newPotVal = ADCH;         //update the variable newPotVal with new value from A0 (between 0 and 255)
}

void loop(){
  checkEncoder();
  checkButton();
  if(newPotVal > oldPotVal + 3 || newPotVal < oldPotVal - 3){
    Serial.println("Pot value: " + String(newPotVal));
    oldPotVal = newPotVal;
  }
}

void checkButton(){

  int buttonVal = digitalRead(12);
  if (buttonVal == LOW){
    Serial.println("Button Pressed!");
  }
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


