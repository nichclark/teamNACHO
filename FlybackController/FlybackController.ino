// PID Loop Program


int PWMOutput = 6;
int pot = A0;
int feedback = A1;
/*-------------------------------------------*/
int kP = 1.9;
int kD = .95;
int kI = 0.25;
int IntegralRange = 25;
long SampleTime = 25; //miliseconds
/*-------------------------------------------*/
//int PTerm, ITerm, SumTerm;
//int Error, NewError;
//int PrevError = 0;
//int Desired, Actual;



void setup() {
  TCCR0B = TCCR0B & B11111000 | B00000001;     // Prescale factor of 1 to get a Fpwm = 62.5kHz
  Serial.begin(9600);
  millis();
}

void loop() {
  int DutyCycle;
  long LastTime;
  
  
  long CurrentTime = millis()/64;
  
  if(CurrentTime >= LastTime + SampleTime){
    DutyCycle = PIDcontroller();
    LastTime = CurrentTime;
  }
  analogWrite(PWMOutput, DutyCycle);

}
int PIDcontroller(){
  int Last;
  int Integral;
  int Desired = analogRead(pot);    //pot value between 0 and 1024
  int Actual = analogRead(feedback); //feedback from output voltage
  int Error = Desired - Actual;   //calculate the error
  
  if(abs(Error) < IntegralRange){
    Integral = Integral + Error;
  }
  else{
    Integral = 0;
  }
  
  int P = Error * kP;   //multiply error by proportional term
  int I = Integral * kI;
  int D = (Last - Actual) * kD;
  int Sum = P + I + D;
 
  int Duty = map(Sum, 0, 1024, 0, 255);
  if(Duty > 255){
    Duty = 255;
  }
  if(Duty < 0){
    Duty = 0;
  }
  Last = Actual;
  Serial.println(Duty);
  return Duty;
}

