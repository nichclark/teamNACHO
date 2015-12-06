// PID Loop Program

/*
For 1V:
kP = 1.6
kI = 1
kD = 1

50ohm = 74/255 duty
68ohm = 71/255 duty
100ohm = 67/255 duty
200ohm = 60/255 duty
5kohm = couldn't get with in 50mV tolerance

For 5V:
kP = 1.4
kI = 1
kD = 1

50ohm = 144/255 duty
68ohm = 138/255 duty
100ohm = 131/255 duty
200ohm = 128/255 duty
5kohm = couldn't get with in 250mV tolerance (105/255)=5.45V

For 12V:
kP = 1.4
kI = 1
kD = 1

50ohm = /255 duty
68ohm = /255 duty
100ohm = /255 duty
200ohm = -/255 duty
5kohm = couldn't get with in 250mV tolerance (105/255)=5.45V

*/

int PWMOutput = 6;
int pot = A0;
int feedback = A1;
long LastTime = 0;
int OutputSignal = 0;
int Last = 0;
int Integral = 0;
int DutyCycle = 0;
int Error = 0;
int P = 0;
int I = 0;
int D = 0;
int Duty = 0;
int Actual = 0;
int Desired = 0;

/*-------------------------------------------*/
int kP = 1.05;
int kI = 0;
int kD = 1;
int IntegralRange = 500;
long SampleTime = 25; //miliseconds
/*-------------------------------------------*/

void setup() {
  TCCR0B = (TCCR0B & 0b11111000) | 0x01;
  Serial.begin(9600);
  
}
  
void loop() {
  int OutputSignal;
  long CurrentTime = millis()/64;
  Desired = analogRead(pot);
  Actual = analogRead(feedback);  
  
  if(CurrentTime - LastTime > SampleTime){
    /*OutputSignal = PIDcontroller(Desired,Actual);
    */

    Error = Desired - Actual;   //calculate the error
    Serial.print("Error equals ");
    Serial.print(Error);Serial.print(", ");
    if(abs(Error) < IntegralRange){
      Integral = Integral + Error;
    }
    else{
      Integral = 0;
    }
    P = Error * 1.1;//multiply error by proportional term
    I = Integral * 1;
    D = (Last - Actual) * 1;
    Duty = P + I + D;
  
    if(Duty > 1000){
      Duty = 1000;
    }
    if(Duty < 0){
      Duty = 0;
    } 
    // map fxn takes a number between 0 and 1024 and converts it
    // to be between 0 and 255
    DutyCycle = map(Duty, 0, 1024, 0, 255); 
    
    Last = Actual;
    LastTime = CurrentTime;
    Serial.print("Output Duty = ");
    Serial.print(Duty);Serial.print(", ");
    Serial.println(DutyCycle);
  }
  analogWrite(PWMOutput, DutyCycle);
  
}
/*
int PIDcontroller(int Desired, int Actual){
  int Last;
  int Integral;
  int Error = Desired - Actual;   //calculate the error
  Serial.print("Error equals ");
  Serial.println(Error);
  if(abs(Error) < IntegralRange){
    Integral = Integral + Error;
  }
  else{
    Integral = 0;
  }
  
  int P = Error * kP;   //multiply error by proportional term
  int I = Integral * kI;
  int D = (Last - Actual) * kD;
  int Duty = P + I + D;

  if(Duty > 225){
    Duty = 225;
  }
  if(Duty < 0){
    Duty = 0;
  } 
  int DutyCycle = map(Duty, 0, 1024, 0, 255);
  
  Last = Actual;
  Serial.print("DutyCycle equals ");
  Serial.print(Duty);Serial.print(",");Serial.println(DutyCycle);
  return DutyCycle;
}
*/
