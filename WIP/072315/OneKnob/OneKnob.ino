#include <Encoder.h>

Encoder eKnob(2, 3);

void setup() {
  Serial.begin(9600);
}

long oldKnobPos  = -999;

void loop() {
  long newKnobPos;
  newKnobPos = eKnob.read();
  if (newKnobPos != oldKnobPos) {
    Serial.print("Left = ");
    Serial.print(newKnobPos);
    Serial.println();
    oldKnobPos = newKnobPos;
  }
}
