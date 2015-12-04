// PID Loop Program


#include "Wire.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();

#define constFive 5
//#define PWMOutput 6

//#define feedback 13

#define button2 7
#define button 8


int PWMOutput = 6;
int pot = A0;
int feedback = A1;

// A4 = DAT
// A5 = CLC


/*-------------------------------------------*/
int kP = 1.9;
int kD = .95;
int kI = 0.25;
int IntegralRange = 25;
long SampleTime = 25; //miliseconds
/*-------------------------------------------*/


void setup() {
}
  
void loop() {
  double DutyCycle;
  long LastTime;
  int CurrentTime = millis()/64;

  //int num = WhatTheNumber();  //num is in "centi"volts
  //displayValue(num);
  //double Desired = (((num/100)/3)*1023)/5; //Convert num to 1024
  int Desired = analogRead(0);
  double Actual = calcFeedback();
  //Serial.println(feedback);
  Serial.println(analogRead(0));
  
  
  if(CurrentTime >= LastTime + SampleTime){
    DutyCycle = PIDcontroller(Desired,Actual);
    LastTime = CurrentTime;
  }
  analogWrite(PWMOutput, DutyCycle);
  //analogWrite(constFive, (165));


}




int calcFeedback(){

  //Reads a PWM signal's duty cycle and frequency.
  static double duty;
  static double Vfb;
  static long highTime = 0;
  static long lowTime = 0;
  static long tempPulse;
  
  tempPulse = pulseIn(feedback,HIGH);
  if(tempPulse>highTime){
    highTime = tempPulse;
  }
  tempPulse = pulseIn(feedback,LOW);
  if(tempPulse>lowTime){
    lowTime = tempPulse;
  }
  duty = highTime/(double (lowTime+highTime));

  Vfb = 255*duty;
  Vfb = map(Vfb, 0, 255, 0, 1024);
  
  return Vfb;
}

int PIDcontroller(double Desired, double Actual){
  double Last;
  double Integral;
  double Error = Desired - Actual;   //calculate the error
  
  if(abs(Error) < IntegralRange){
    Integral = Integral + Error;
  }
  else{
    Integral = 0;
  }
  
  double P = Error * kP;   //multiply error by proportional term
  double I = Integral * kI;
  double D = (Last - Actual) * kD;
  double Duty = P + I + D;
 
  Duty = map(Duty, 0, 1024, 0, 255);
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

/*void displayValue (int value){
  matrix.print(value,DEC);
  //matrix.writeDigitNum(1,true);
  matrix.blinkRate(0);
  matrix.drawColon(true);
  matrix.writeDisplay();
}*/

int WhatTheNumber (){
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
  
  int maxValue = 1250;
  int minValue = 50;
  int reading =! digitalRead(button);
  int reading2 =! digitalRead(button2);
  
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
