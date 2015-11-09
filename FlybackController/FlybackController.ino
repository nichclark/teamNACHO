// PID Loop Program


#define PWMOutput = 6;
#define pot = A0;
#define feedback = A1;

#define READ_PIN 13

// A4 = DAT
// A5 = CLC

//////////////////////////////////////////////////
int button = 8;
int button2 = 7;

int x;

int bounceTime = 50;
int holdTime = 32000;

int lastReading = LOW;
int lastReading2 = LOW;

int hold = 0;
int hold2 = 0;

int single = 0;
int single2 = 0;

long onTime = 0;
long lastSwitchTime = 0;
///////////////////////////////////////////////////
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
  //Serial.println("7 Segment Backpack Test");
  matrix.begin(0x70);
  millis();
  pinMode(button, INPUT);
  pinMode(button2, INPUT);
  pinMode(READ_PIN,INPUT);
}
  
void loop() {
  int DutyCycle;
  long LastTime;
  int CurrentTime = millis()/64;

  int num = WhatTheNumber();  //num is in "centi"volts
  displayValue(num);
  float val = (((num/100)/3)*1023)/5; //Convert num to 1024
  Serial.println(val);
  
  if(CurrentTime >= LastTime + SampleTime){
    DutyCycle = PIDcontroller(val);
    LastTime = CurrentTime;
  }
  analogWrite(PWMOutput, DutyCycle);


}
void displayValue (int value){
  matrix.print(value,DEC);
  //matrix.writeDigitNum(1,true);
  matrix.blinkRate(0);
  matrix.drawColon(true);
  matrix.writeDisplay();
}

int WhatTheNumber (){
  int maxValue = 1250;
  int minValue = 50;
  int reading = digitalRead(button);
  int reading2 = digitalRead(button2);
  
  //first pressed
   if (reading == HIGH && lastReading == LOW && reading2 == LOW) {
     onTime = millis();
     Serial.println("press");
     x=x+10;
   }
  
  //held
   if (reading == HIGH && lastReading == HIGH && reading2 == LOW) {
     if ((millis() - onTime) > holdTime) {
       Serial.println("holding");
       x=x+10; 
       hold = 1;
     }
   }
  
  //released
   if (reading == LOW && lastReading == HIGH) {
     if (hold == 1) {
       Serial.println("let go");
       hold = 0;
     }   
   }
   
   lastReading = reading;
  
  //DECREMENT SETUP
  
  
  //first pressed
   if (reading2 == HIGH && lastReading2 == LOW && reading == LOW) {
     onTime = millis();
     Serial.println("press");
     x=x-10;
   }
  
  //held
   if (reading2 == HIGH && lastReading2 == HIGH && reading == LOW) {
     if ((millis() - onTime) > holdTime) {
       Serial.println("holding");
        x=x-10; 
       hold2 = 1;
     }
   }
  
  //released
   if (reading2 == LOW && lastReading2 == HIGH) {
     if (hold2 == 1) {
       Serial.println("let go");
       hold2 = 0;
     }   
   }
   if(x > maxValue){
    x = maxValue;
   }
   if(x < minValue){
    x = minValue;
   }
   lastReading2 = reading2;
   return x;
}

int PIDcontroller(int val){
  int Last;
  int Integral;
  int Desired = val;
  //int Desired = analogRead(pot);    //pot value between 0 and 1024
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
 //Serial.println(Duty);

  return Duty;
}


void calcfeedback(){

  //Reads a PWM signal's duty cycle and frequency.

  
  
  static double duty;
  static long highTime = 0;
  static long lowTime = 0;
  static long tempPulse;
  
  
  void loop(){
  readPWM(READ_PIN);
  Serial.println(duty);
  }
  
  //Takes in reading pins and outputs pwm duty cycle.
  void readPWM(int readPin){
  highTime = 0;
  lowTime = 0;
  
  tempPulse = pulseIn(readPin,HIGH);
  if(tempPulse>highTime){
  highTime = tempPulse;
  }
  
  tempPulse = pulseIn(readPin,LOW);
  if(tempPulse>lowTime){
  lowTime = tempPulse;
  }
  
  duty = (100*(highTime/(double (lowTime+highTime))));
  }
}
