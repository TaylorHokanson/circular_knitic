/*
 Input Pullup Serial

 This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a
 digital input on pin 2 and prints the results to the serial monitor.

 The circuit:
 * Momentary switch attached from pin 2 to ground
 * Built-in LED on pin 13

 Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal
 20K-ohm resistor is pulled to 5V. This configuration causes the input to
 read HIGH when the switch is open, and LOW when it is closed.

 created 14 March 2012
 by Scott Fitzgerald

 http://www.arduino.cc/en/Tutorial/InputPullupSerial

 This example code is in the public domain

 */

boolean buttonState = true; 
int buttonPin = 2;

void setup() {
  //start serial connection
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(13, OUTPUT);

}

void loop() {
  int sensorVal = digitalRead(buttonPin);
  //Serial.println(sensorVal);
  if (sensorVal == LOW) {
    buttonState = !buttonState;
    digitalWrite(13, buttonState);
  }else{
    ;
  }
}



