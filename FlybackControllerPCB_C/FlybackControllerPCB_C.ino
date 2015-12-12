/* TeamNACHO's "NACHO Power" USB Power Supply- CGrade Code
 *  By: Nicholas Clark & Adam Hastings
 *  Date Complete: 07 December 2015
 *  
 *  Pins being used for CGrade PCB on ATmega328p:
 *  PC5-7seg display/CLK = A5
 *  PC4-7seg display/DAT = A4
 *  PC1-Feedback = A1
 *  PD5-PWM = D5
 *  PD7-Pushbutton1 = D7
 *  PB0-Pushbutton2 = D8
 *  
 */
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
Adafruit_7segment matrix = Adafruit_7segment();

#define maxValue 1250
#define minValue 50

// These variables correspond to pins on the Arduino
int PWMOutput = 5;
int pot = A0;
int feedback = A1;
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
int button = 7;
int button2 = 8;
int bounceTime = 50;
int holdTime = 2500;
int x=0;

int lastReading = HIGH;
int lastReading2 = HIGH;

int hold = 0;
int hold2 = 0;

int single = 0;
int single2 = 0;

long onTime = 0;
long lastSwitchTime = 0;


//The follow varables are what get used in the PID loop
int kP = 0.2; //proportional constant
int kI = 1; //integral constant
int kD = 0; //derivative constant
int IntegralRange = 500; //sets when the intergral term start taking effect
long SampleTime = 50; //how often PID loop runs in miliseconds

void setup() {
  //TCCR0B is the internal clock the program uses. The default scale 
  //factor for this clock is 0x04, making the clock frequency 976.5625 Hz.
  //By changing the scale factor to 0x01, the new clock frequency is
  //62500 Hz. This change in clock speed had to be done so that the output
  //pin, pin6, would be at 62.5kHz. This information is from
  //http://playground.arduino.cc/Main/TimerPWMCheatsheet
  TCCR0B = (TCCR0B & 0b11111000) | 0x01;
  Serial.begin(9600); //Baud rate of serial monitor used for debugging
  matrix.begin(0x70);
  pinMode(5,OUTPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(A1,INPUT);
}

void loop() {
  long CurrentTime = millis()/64;
  Desired = userInput();
  Actual = analogRead(feedback); 

  if(CurrentTime - LastTime > SampleTime){
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
  }
  analogWrite(PWMOutput, 125);
  displayValue(Desired);
  
}
void displayValue (int value){
  int A;int B;int C;int D;
  //value will have a max value of 1250
  //value = map(value,0,1024,minValue,maxValue);
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

int userInput(){
  int reading = digitalRead(button);
  int reading2 = digitalRead(button2);

  //first pressed
  if (reading == LOW && lastReading == HIGH && reading2 == HIGH) {
    onTime = millis();
    x=x+10;
  }
  
  //held
  if (reading == LOW && lastReading == LOW && reading2 == HIGH) {
    if ((millis() - onTime) > holdTime) {
      x=x+10;
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
    x=x-10;
  }
  
  //held
  if (reading2 == LOW && lastReading2 == LOW && reading == HIGH) {
    if ((millis() - onTime) > holdTime) {
      x=x-10;
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
  //x = map(x,minValue,maxValue,0,1024);
  return x;
}


