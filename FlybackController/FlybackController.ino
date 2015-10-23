int ledPin = 6;
int duty = 127;
void setup() {
  // put your setup code here, to run once:
  TCCR0B = TCCR0B & B11111000 | B00000001;     // Prescale factor of 1 to get a Fpwm = 62.5kHz

}

void loop() {
  // put your main code here, to run repeatedly:
   analogWrite(ledPin, duty);
     
}
