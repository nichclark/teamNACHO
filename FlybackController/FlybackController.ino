// PID Loop Program


int PWMOutput = 6;
int pot = A0;
int feedback = A1;
// A4 = DAT
// A5 = CLC
int ButtonUP = 8;
int ButtonDOWN = 7;
int x=0;

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

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();


void setup() {
  TCCR0B = TCCR0B & B11111000 | B00000001;     // Prescale factor of 1 to get a Fpwm = 62.5kHz
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
  matrix.begin(0x70);
  millis();
  pinMode(ButtonUP, INPUT);
  pinMode(ButtonDOWN, INPUT);
}
  
void loop() {
  int DutyCycle;
  long LastTime;
  int CurrentTime = millis()/64;
  
  int lastState1 = LOW;   //previous current state
  int lastState2 = LOW;

//variables are longs since time is measured in milliseconds
  long lastDebounceTime = 0;  //last time the output pin was toggled
  long debounceDelay = 50;    //the debounce time; increase is output flickers

  
//Read state of each switch, store in variable.  
  int buttonState1 = digitalRead(ButtonUP);  //reading
  int buttonState2 = digitalRead(ButtonDOWN);

  
  
  if (buttonState1 != lastState1){
    lastDebounceTime = CurrentTime;
  }
  if (buttonState2 != lastState2){
    lastDebounceTime = CurrentTime;
  }

  if ((CurrentTime - lastDebounceTime) > debounceDelay){
    if (buttonState1 != lastState1) {
      lastState1 = buttonState1; 
    }
    if (buttonState2 != lastState2) {
      lastState2 = buttonState2; 
    }
  }

  if (buttonState1 == HIGH){
    x=x+1;
  }

Serial.println(x);
  
  // Testing the 7-seg display
  matrix.print(x,DEC);
  //matrix.writeDigitNum(1,2,true);
  matrix.blinkRate(0);
  matrix.writeDisplay();
  
  
  
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
 // Serial.println(Duty);
  return Duty;
}

