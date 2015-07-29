/******************************************
	PURPOSE:	Tutorial on comparator interrupt use at the example of the microphone
	Created by      Rudy Schlaf for www.makecourse.com
	DATE:		3/2014
*******************************************/


#define referenceVoltagePin 3 //this pin is fed into AIN0 (pin D6) filtered with 10k/10u to provide a reference voltage
#define MicrophoneGNDpin 14   //define pin 14 (=A0) as OUTPUT to be used as GND for the microphone
#define Microphone5Vpin 15    //define pin 15 (=A1) as OUTPUT to be used as 5V supply for the microphone
#define MicrophoneOutputPin 16//define pin 16 (=A2) as INPUT  for the microphone signal to be fed into the comparator

#define thresholdvalue 150//this defines the reference voltage fed into pin D6: 0=0V and 255=5V;
                          //the microphone puts out a level of about 105 (on the 0->255 scale), i.e. this is a bit higher

//this defines two convenient commands that set or clear individual bits in a register.
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))    //clear bit in byte at sfr adress
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))     //set bit in byte at sfr adress

//variables used in the Interrupt Service Routine (ISR) need to be 'volatile'
volatile byte sound=0;
volatile long unsigned soundTime;

void setup(){
  
  Serial.begin(9600);//start serial communication
  pinMode(MicrophoneGNDpin, OUTPUT);//define pin 14 (=A0) as OUTPUT
  digitalWrite(MicrophoneGNDpin, LOW);//A0 is GND for microphone
  pinMode(Microphone5Vpin, OUTPUT);//define pin 15 (=A1) as OUTPUT
  digitalWrite(Microphone5Vpin, HIGH);//A1 is 5V supply for microphone
  pinMode(MicrophoneOutputPin, INPUT);//pin 16 (=A2) is used as INPUT pin for the microphone signal
  
  pinMode(referenceVoltagePin, OUTPUT);//this pin is used as pulse width modulated (PWM) output pin to generate the external reference voltage fed into pin D6
  analogWrite(referenceVoltagePin, thresholdvalue);//set the reference comparator voltage for AIN0

  //*****the following cryptic commands set the relevant bits in the Arduino processor to enable the comparator interrupt process:*********
  
  //The sbi and cbi commands set and clear bits in the registers. The syntax is sbi(register address, bit number) - see definitions above
  //the acronyms ADCSRB are #defined in Arduino.h and define register addresses or bit numbers. They are directly from the datasheet, this makes programming these registers fairly easy.
  //further details: get the Atmega328p datasheet and read Chapter 22. Analog Comparator
  
  sbi(ADCSRB,ACME);//Select Multiplexer output as negative input for comparator
  cbi(ADMUX,MUX2);//set mux bits 2,1,0 in the ADMUX register to 010 (this selects A2 as MUX input; A2 is used to read out the microphone)
  sbi(ADMUX,MUX1);
  cbi(ADMUX,MUX0);
  cbi(ADCSRA,ADEN);//Turn ADC off - this is a condition for using the MUX for selecting the comparator input - see Chapter 23 of the datasheet for more details on the MUX
  //the following statement is an alternate way to use the acronyms if an entire byte is to be set. sbi/cbi only affect the chosen byte, the statement below affects all 8 bits.
  //see chapter 22 and http://forum.arduino.cc/index.php/topic,17450.0.html (a thread about using the comparator interrupt with some interesting bits of information)
  ACSR = 
   (0<<ACD) |   // Analog Comparator: Enabled
   (0<<ACBG) |   // Analog Comparator Bandgap Select: AIN0 (pin D6) is used as positive comparator input - here we define the threshold that triggers the interrupt
   (0<<ACO) |   // Analog Comparator Output: Off
   (1<<ACI) |   // Analog Comparator Interrupt Flag: Clear Pending Interrupt
   (1<<ACIE) |   // Analog Comparator Interrupt: Enabled
   (0<<ACIC) |   // Analog Comparator Input Capture: Disabled
   (0<<ACIS1) | (0<ACIS0);   // Analog Comparator Interrupt Mode: Comparator Interrupt on Output Toggle (see Table 22-2 in the data sheet)

}

long unsigned previousSoundTime;

void loop(){
  if (sound == 1){//if the sound flag was set in the ISR...
    float timeSinceLastSound = soundTime - previousSoundTime;//calculate the time since the last event
    previousSoundTime=soundTime;//note the present event time as last time for the next event.
    Serial.print(timeSinceLastSound/1000);//print the elapsed time on the serial monitor
    Serial.println(" seconds");
    delay(100);//each clap causes several interrupts due to echo, waiting for 0.1 sec before resetting the sound flag takes care of this 
               //(this could be done much better w/o the delay wait, check out the 'switches as input devices 'in class project' video on makecourse.com)
    sound = 0;//reset the flag for the next sound event
  }
}

ISR(ANALOG_COMP_vect)//this is the Interrupt Service Routine; "ANALOG_COMP_vect" is a #defined expression specifying the interrupt vector associated with the comparator interrupt
 {
   soundTime=millis();//note the time of the ISR call   
   sound=1; // Sound detected
 }

  
