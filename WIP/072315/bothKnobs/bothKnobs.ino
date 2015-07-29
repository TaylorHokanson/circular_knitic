#include <Encoder.h>

Encoder eKnob(2, 3);

void setup() {
  Serial.begin(9600);
}

long oldKnobPos  = -999;
float oldPotValue = 1000;
long newKnobPos = 0;
float newPotValue = 0;
  
void loop() {

  newKnobPos = eKnob.read();
  newPotValue = analogRead(A0);
  float voltage = newPotValue * (5.0 / 1023.0);
   
  if (newKnobPos != oldKnobPos) {
    Serial.print("Encoder = ");
    Serial.print(newKnobPos);
    Serial.println();
    oldKnobPos = newKnobPos;
  }

  // not sure why this constantly reports, something to do with interrupts?
  if (newPotValue != oldPotValue){
    Serial.print("Pot = ");
    Serial.print(voltage);
    Serial.println();
    oldPotValue = newPotValue;
  }
  delay(10);
}
