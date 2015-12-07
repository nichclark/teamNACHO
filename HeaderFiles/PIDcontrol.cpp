#include "PIDcontrol.h"
#include <arduino.h>
#include <avr/io.h>


int PIDcontrol(int kP,int kI,int kD,int IntegralRange){
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
	
	//Calculate the error in the output voltage
	Error = Desired - Actual;
	//Determine if the Integral should be used in the PID calculation.
	//The integral is found by adding the new error to the prevous error
	//By having a range on the Integral, this prevents the integral
	//value from getting too large.
	if(abs(Error) < IntegralRange) Integral = Integral + Error;
	else Integral = 0;
	//The terms of the PID loop are calculated below:
	//Multiply error by proportional constant
	P = Error * kP;
	//Multiply the integral term by the integral constant
	I = Integral * kI;
	//Determine the differenece between the current user input value and
	//the last user input. This is then multiplied by the derivative constant
	D = (Last - Actual) * kD;
	//Add the three terms together to determine the output duty cycle
	Duty = P + I + D;
	//A max and min limit were set on the output duty cycle. This was done
	//to protect the harware.
	if(Duty > 1000) Duty = 1000;
	if(Duty < 0) Duty = 0;
	//The map fxn takes a number between 0 and 1024 and converts it
	//to be between 0 and 255. This is needed as the analogWrite fxn
	//needs a value between 0 and 255.
	DutyCycle = map(Duty, 0, 1024, 0, 255);
	//Save the current user input to be used to calculate the derivative
	Last = Actual;
	Serial.print("Error equals ");
	Serial.print(Error);Serial.print(", ");
	Serial.print("Output Duty = ");
	Serial.print(Duty);Serial.print(", ");
	Serial.println(DutyCycle);
}