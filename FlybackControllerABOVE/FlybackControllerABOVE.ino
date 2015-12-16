//Include the following header files
/*
#include "inits.h"
#include "display.h"
#include "userinput.h"
#include "PIDcontrol.h"
#include "feedback.h"
*/
#include "Wire.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
Adafruit_7segment matrix = Adafruit_7segment();

//These variables correspond to pins on the Arduino
#define Switch 4 //pin D5
#define Vfeedback 6 //pin D6
#define PBa 7 //pin D7
#define PBb 8 //pin D8
#define PWMOutput  9 //pin D9
#define ConstOutput  10 //pin D10
//pin A4 = DAT for 7segDisplay
//pin A5 = CLK for 7segDisplay

//Define variables that set the max range of the power supply (x100)
int maxValue = 1250;
int minValue = 50;
// These global variables are initialized and set to zero


void setup() {
  TCCR1B = (TCCR1B & 0b11111000) | 0x01;
  
  //Baud rate of serial monitor used for debugging
  Serial.begin(9600);
  matrix.begin(0x70);
  
  millis();

  pinMode(Switch,INPUT);
  pinMode(Vfeedback,INPUT_PULLUP); // feedback pin
  pinMode(PBa,INPUT); //PB1
  pinMode(PBb,INPUT); //PB2
  pinMode(PWMOutput,OUTPUT); //Flyback gate PWM
  pinMode(ConstOutput,OUTPUT); //Constant gate PWM
}


void loop() {
/**/// PID Loop related variables
  float kP = 1; //proportional constant
  float kI = 0; //integral constant
  float kD = 0; //derivative constant
/**/// Time related variables
  long CurrentTime = millis();
  long LastTime;
  long SampleTime = 25; //how often PID loop runs in miliseconds
/**/// PWM related variables
  double DutyCycle; float Desired; float Actual;
  double SetDuty = 160;

  //double UserPick = userInput();
  double UserPick = 500;

  analogWrite(PWMOutput, DutyCycle);
  //analogWrite(PWMOutput, 130);
  analogWrite(ConstOutput,SetDuty);
  displayValue(UserPick);
  float feedback = calcFeedback(Switch,Vfeedback);
  if(CurrentTime > LastTime + SampleTime){
    Desired = map(UserPick,0,1500,0,1023); 
    Actual = feedback;
    //if(digitalRead(Vfeedback) == HIGH) DutyCycle = map(Desired, 0, 1023, 0, 255);
    DutyCycle = PIDcontrol(Desired,Actual,kP,kI,kD);
    LastTime = CurrentTime;
  }
  //Serial.println(DutyCycle);
}


void displayValue (int value){
  int A;int B;int C;int D;
  A = value/1000;
  B = (value/100) - (A*10);
  C = (value/10) - (A*100) - (B*10);
  D = (value/1) - (A*1000) - (B*100) - (C*10);
  
  matrix.writeDigitNum(0,A);
  matrix.writeDigitNum(1,B,true);
  matrix.writeDigitNum(3,C);
  matrix.writeDigitNum(4,D);
  matrix.writeDisplay();
}


float calcFeedback(int sw, int pin){
  float standByV = 0.3;
  long lowTime = 0;
  float dutyPercent;
  float value;
  int i;
  if(digitalRead(sw) == LOW) value = standByV * 1023 / 5;
  else{
    lowTime = pulseIn(pin, LOW, 10000);
    dutyPercent = 1 - ((float)lowTime / 4000);
    value = 1023*dutyPercent;
  }
/*
  Serial.print("Low = ");Serial.print(lowTime);Serial.print(", ");
  Serial.print("Percent = ");Serial.print(dutyPercent);Serial.print(", ");
  Serial.print("value = ");Serial.println(value);
*/
  return value;
}


double userInput(){
  int bounceTime = 50;
  int holdTime = 1000;
  int lastReading = HIGH;
  int lastReading2 = HIGH;
  int hold = 0;
  int hold2 = 0;
  int single = 0;
  int single2 = 0;
  long onTime = 0;
  double x;
  ////////////////////////////////////////////
  int reading = digitalRead(PBa);
  int reading2 = digitalRead(PBb);

  //first pressed
  if (reading == LOW && lastReading == HIGH && reading2 == HIGH) {
    onTime = millis();
    x=x+5;
  }
  
  //held
  if (reading == LOW && lastReading == LOW && reading2 == HIGH) {
    if ((millis() - onTime) > holdTime) {
      x=x+5;
      hold = 1;
    }
  }
  
  //released
  if (reading == HIGH && lastReading == LOW) {
    if (hold == 1) hold = 0;
  }
  
  lastReading = reading;
  
  //DECREMENT SETUP
  
  
  //first pressed
  if (reading2 == LOW && lastReading2 == HIGH && reading == HIGH) {
    onTime = millis();
    x=x-5;
  }
  
  //held
  if (reading2 == LOW && lastReading2 == LOW && reading == HIGH) {
    if ((millis() - onTime) > holdTime) {
      x=x-5;
      hold2 = 1;
    }
  }
  
  //released
  if (reading2 == HIGH && lastReading2 == LOW) {
    if (hold2 == 1) hold2 = 0;
  }
  if(x > maxValue) x = maxValue;
  if(x < minValue) x = minValue;
  lastReading2 = reading2;
  //Serial.println(x);
  return x;
}


float PIDcontrol(float Desired, float Actual, float kP, float kI, float kD){
  float maxOut = 1000;
  float minOut = 0;
  
  float lastDesired;
  float Error = Desired - Actual;

  float P = (Error * kP);
  
  float I = I + (Error * kI);
  //if(I > maxOut) I = maxOut;
  //if(I < minOut) I = minOut;
  
  float D = (Desired - lastDesired ) * kD;
  
  float Out = P - D;
  if(Out > maxOut) Out = maxOut;
  if(Out < minOut) Out = minOut;
  
  float Duty = map(Out, 0, 1023, 0, 255);
  
  lastDesired = Desired;
/**/
  Serial.print("Des ");Serial.print(Desired);Serial.print(", ");
  Serial.print("Act ");Serial.print(Actual);Serial.print(", ");
  Serial.print("Error ");Serial.print(Error);Serial.print(", ");
  Serial.print("P ");Serial.print(P);Serial.print(", ");
  Serial.print("I ");Serial.print(I);Serial.print(", ");
  Serial.print("D ");Serial.print(D);Serial.print(", ");
  Serial.print("Duty # ");Serial.println(Duty);

/**/  
  return Duty;
}


