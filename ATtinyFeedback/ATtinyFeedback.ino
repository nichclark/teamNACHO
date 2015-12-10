/*
ATtiny85 PinOut
      Reset |1   o   8| VCC+
AI3/DO-Pin3 |2       7| AI1-Pin2
AI2/DO-Pin4 |3       6| PWM-Pin1
        GND |4       5| PWM-Pin0

*/
int Out = 0;
int FBack = 3;
void setup() {
  analogReference(DEFAULT);
  pinMode(Out, OUTPUT);
  //pinMode(FBack, INPUT);
}
void loop() {
  int value = analogRead(FBack);
  int duty = value/4;
  analogWrite(Out,duty);
}

