
// These variables correspond to pins on the Arduino

int pot = A0;
int feedback = A1;
int Switch = 3;
int Vopto = 4;
int PWMOutput = 6;
// These global variables are initialized and set to zero
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

//The follow varables are what get used in the PID loop
int kP = 0.2; //proportional constant
int kI = 1; //integral constant
int kD = 0; //derivative constant
int IntegralRange = 500; //sets when the intergral term start taking effect
long SampleTime = 50; //how often PID loop runs in miliseconds

#include "feedback.h"
int var = 0;


void setup() {

  TCCR0B = (TCCR0B & 0b11111000) | 0x01;
  TCCR1B = (TCCR1B & 0b11111000) | 0x01;
  Serial.begin(9600); 
  pinMode(Vopto,INPUT);
 // pinMode(5,OUTPUT);
  //pinMode(6,OUTPUT);
}
  
void loop() {
  //long CurrentTime = millis()/64;
  //Desired = analogRead(pot);
  var = 125;
  int Actual = calcFeedback(Vopto); 
 
  analogWrite(PWMOutput, var);
  analogWrite(5, 170);

  Serial.print("Desired equals ");
  Serial.print(var);Serial.print(", ");
  Serial.print("Vopto = ");
  Serial.println(Actual);
}
 /*if(CurrentTime - LastTime > SampleTime){
   
    Error = Desired - Actual;   
    if(abs(Error) < IntegralRange) Integral = Integral + Error;
    else Integral = 0;
    P = Error * kP;
    I = Integral * kI;
    D = (Last - Actual) * kD;
    Duty = P + I + D;
    if(Duty > 1000) Duty = 1000;
    if(Duty < 0) Duty = 0; 
    DutyCycle = map(Duty, 0, 1024, 0, 255); 
    Last = Actual;
    LastTime = CurrentTime;

    Serial.print("Error equals ");
    Serial.print(Error);Serial.print(", ");
    Serial.print("Output Duty = ");
    Serial.print(Duty);Serial.print(", ");
    Serial.println(DutyCycle);
  }*/

