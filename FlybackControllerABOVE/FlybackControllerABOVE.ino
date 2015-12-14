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
int SetDuty = 160;
// These global variables are initialized and set to zero
int DutyCycle = 0;
long CurrentTime = 0;
long LastTime = 0;
int Desired = 0;
int Actual = 0;

long pwm_value;
long sum;
long avg;
double total;
double value;
long final[20];
int i;
int j;
int maxx = 0;

int bounceTime = 50;
int holdTime = 25000;
int x=0;
int lastReading = HIGH;
int lastReading2 = HIGH;
int hold = 0;
int hold2 = 0;
int single = 0;
int single2 = 0;
long onTime = 0;
long lastSwitchTime = 0;

int Last = 0;
int Integral = 0;
int Error = 0;
int P = 0;
int I = 0;
int D = 0;
int Duty = 0;

//The follow varables are what get used in the PID loop
float kP = 0.2; //proportional constant
float kI = 0.2; //integral constant
float kD = 0; //derivative constant
int IntegralRange = 500; //sets when the intergral term start taking effect
int SampleTime = 25; //how often PID loop runs in miliseconds

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
  // Create arrray for the feedback calculation function to use
  for (i=0; i<20; i++) final[i] = 0;
}
void loop() {
  CurrentTime = millis();
  //Serial.println(CurrentTime);
  Desired = userInput();
  //Desired = map(5,0,1250,0,1024);
  Desired = 600;
  displayValue(150);
  if(CurrentTime > LastTime + SampleTime){
    //Actual = calcFeedback();
    for(i=0; i<20; i++){ 
     do{pwm_value = pulseIn(6, LOW);} while(pwm_value < 10);
      final[i] = pwm_value;
      if (i==19){
        for (j=0; j<20; j++){
          if(final[j] > maxx){maxx = final[j];}}}}
    total = maxx/40;
    Actual = 1024*(100-total)/100;

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
    Serial.print("Desired = ");Serial.print(Desired);Serial.print(", ");
    Serial.print("Actual = ");Serial.print(Actual);Serial.print(", ");
    Serial.print("Error = ");Serial.print(Error);Serial.print(", ");
    Serial.print("Output Duty = ");Serial.print(Duty);Serial.print(", ");Serial.println(DutyCycle);

  
    
    LastTime = CurrentTime;
    maxx=0;
  }
  analogWrite(PWMOutput, DutyCycle);
  analogWrite(ConstOutput,SetDuty);
  
}
void displayValue (int value){
  int A;int B;int C;int D;
  //value will have a max value of 1250
  value = map(value,0,1024,minValue,maxValue);
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


int calcFeedback(){
  
  for(i=0; i<20; i++){
    do
    {
      pwm_value = pulseIn(6, LOW);
    } while(pwm_value < 10);

    final[i] = pwm_value;

    if (i==19){
      for (j=0; j<20; j++){
        if(final[j] > maxx){
          maxx = final[j];
        }
     }
    }  
  }
  //Serial.println(maxx);
  total = maxx/40;
  
  value = 1024*(100-total)/100;
  maxx=0;
  
  return (int) value;
}


int userInput(){

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
  return x;
}


void PIDcontrol(){
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
  Serial.print("Desired = ");Serial.print(Desired);Serial.print(", ");
  Serial.print("Actual = ");Serial.print(Actual);Serial.print(", ");
  Serial.print("Error = ");Serial.print(Error);Serial.print(", ");
  Serial.print("Output Duty = ");Serial.print(Duty);Serial.print(", ");Serial.println(DutyCycle);
}


