//Include the following header files
//#include "inits.h"
/*#include "display.h"*/
//#include "userinput.h"
//#include "PIDcontrol.h"
//#include "feedback.h"

#include "Wire.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
Adafruit_7segment matrix = Adafruit_7segment();

//These variables correspond to pins on the Arduino
int Vfeedback = 6; //pin D6
int PBa = 7; //pin D7
int PBb = 8; //pin D8
int PWMOutput = 9; //pin D9
int ConstOutput = 10; //pin D10
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
  millis();
  
  pinMode(6,INPUT_PULLUP); // feedback pin
  pinMode(7,INPUT); //PB1
  pinMode(8,INPUT); //PB2
  pinMode(9,OUTPUT); //Flyback gate PWM
  pinMode(10,OUTPUT); //Constant gate PWM
  matrix.begin(0x70);

}
void loop() {
  double DutyCycle;
  double SetDuty = 160;
  long LastTime;
  long SampleTime = 25; //how often PID loop runs in miliseconds
  long CurrentTime = millis();
  //double UserPick = userInput();
  double UserPick = 500;
  double Desired = map(UserPick,0,1500,0,1024); //on actual circuit, Vout = [9.8k/(19.6k+9.8k)]Vin = 0.333Vin
  double Actual = calcFeedback();
  if(CurrentTime > LastTime + SampleTime){
    DutyCycle = PIDcontrol(Desired,Actual);
    LastTime = CurrentTime;
  }
  analogWrite(PWMOutput, 127);
  analogWrite(ConstOutput,SetDuty);
  
  
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


double calcFeedback(){
  long lowTime = pulseIn(6, LOW, 5000);
  double dutyPercent = 1-((float)lowTime / 4000);
  double value = 1024*dutyPercent;

  Serial.print("Low = ");Serial.print(lowTime);Serial.print(", ");
  Serial.print("Percent = ");Serial.print(dutyPercent);Serial.print(", ");
  Serial.print("value = ");Serial.println(value);
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


double PIDcontrol(double Desired, double Actual){
  //The follow varables are what get used in the PID loop
  float kP = 0.2; //proportional constant
  float kI = 0.2; //integral constant
  float kD = 0; //derivative constant
  double IntegralRange = 500; //sets when the intergral term start taking effect
  double Integral;
  double Last;
  double Error = Desired - Actual;
  if(abs(Error) < IntegralRange) Integral = Integral + Error;
  else Integral = 0;
  double P = Error * kP;
  double I = Integral * kI;
  double D = (Last - Actual) * kD;
  double Duty = P + I + D;
  if(Duty > 1000) Duty = 1000;
  if(Duty < 0) Duty = 0;
  double DutyCycle = map(Duty, 0, 1024, 0, 255);
  Last = Actual;
  /*
  Serial.print("Desired = ");Serial.print(Desired);Serial.print(", ");
  Serial.print("Actual = ");Serial.print(Actual);Serial.print(", ");
  Serial.print("Error = ");Serial.print(Error);Serial.print(", ");
  Serial.print("Output Duty = ");Serial.print(Duty);Serial.print(", ");Serial.println(DutyCycle);
  */
  return DutyCycle;
}


