// PID Loop Program


//int ledPin = 7;
int pot = A0;
double kp = 1;
double ki = 10;
double PTerm, ITerm;
double Error;
double Desired, Actual;
double DutyCycle;

void setup() {
//  TCCR0B = TCCR0B & B11111000 | B00000001;     // Prescale factor of 1 to get a Fpwm = 62.5kHz

}

void loop() {
//   analogWrite(ledPin, duty); 

  Desired = analogRead(pot);    //pot value between 0 and 1024
  Actual = analogRead(feedback); //feedback from output voltage
  Error = Desired - Actual;   //calculate the error

  PTerm = Error * kp;   //multiply error by proportional term
  
  
  










}
