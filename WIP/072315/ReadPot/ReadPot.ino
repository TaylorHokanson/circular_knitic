int newSensorValue;
//int oldSensorValue;

void setup() {
  Serial.begin(9600);
//  oldSensorValue = analogRead(A0);
//  delay(10);
}

void loop() {
  newSensorValue = analogRead(A0);
//  if (newSensorValue == oldSensorValue){
    float voltage = newSensorValue * (5.0 / 1023.0);
    Serial.println("Pot value: " + String(voltage) + "\n");
    delay(100);
//  }
}
