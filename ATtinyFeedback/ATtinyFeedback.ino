/*
TITLE("TeamNACHO USB Power Supply");
SUBTITLE("Design Review ATtiny Code");
BY("Adam S. Hastings && Nicholas P. Clark");
DATE("17 December 2015");

This code is IMPLEMENTED on an ATtiny thats on the secondary
side of the circuit. The microcontroller analogReads the 
output voltage of the cirucit. It uses this value to set
the duty cycle of a PWM signal. This signal is then
transmitted through hardware to the main microcontroller.

ATtiny85 PinOut
      Reset |1   o   8| VCC+
AI3/DO-Pin3 |2       7| AI1-Pin2
AI2/DO-Pin4 |3       6| PWM-Pin1
        GND |4       5| PWM-Pin0
*/
int Out = 0;
int FBack = 3;
void setup() {
  //Have analog fxn reference VCC
  analogReference(DEFAULT);
  pinMode(Out, OUTPUT);
  //pinMode(FBack, INPUT);
}
void loop() {
  int value = analogRead(FBack);
  int duty = value/4;
  analogWrite(Out,duty);
}

