/*
TITLE("TeamNACHO USB Power Supply");
SUBTITLE("userInput Fxn");
BY("Adam S. Hastings && Nicholas P. Clark");
DATE("17 December 2015");

This code should be IMPLEMENTED to determine what the desired
output voltage is. The fxn uses the two pushbuttons from the
circuit to increment and decrement a variable. This variable
is then returned to be used in TeamNACHO's displayValue fxn
and TeamNACHO's PID Control fxn.
*/

#include "userinput.h"
#include <arduino.h>
#include <avr/io.h>

//Initialize the starting values for these variables
int lastReading = HIGH;
int lastReading2 = HIGH;
float x;

float userInput(int pushbuttA, int pushbuttB){
	int hold = 0;
	int hold2 = 0;
	int single = 0;
	int single2 = 0;
	long onTime = 0;
	
	int bounceTime = 50;
	int holdTime = 2000;
	int maxValue = 1250;
	int minValue = 50;
	
	//Read the two pushbuttons
	int reading = digitalRead(pushbuttA);
	int reading2 = digitalRead(pushbuttB);
	
	//INCREMENT SETUP
	//first pressed
	if (reading == LOW && lastReading == HIGH && reading2 == HIGH) {
		onTime = millis(); x=x+5;
	}
	
	//held
	if (reading == LOW && lastReading == LOW && reading2 == HIGH) {
		if ((millis() - onTime) > holdTime) {x=x+5; hold = 1;}
	}
	
	//released
	if (reading == HIGH && lastReading == LOW) {
		if (hold == 1) hold = 0;
	}

	//DECREMENT SETUP
	//first pressed
	if (reading2 == LOW && lastReading2 == HIGH && reading == HIGH) {
		onTime = millis(); x=x-5;
	}
	
	//held
	if (reading2 == LOW && lastReading2 == LOW && reading == HIGH) {
		if ((millis() - onTime) > holdTime) {x=x-5; hold2 = 1;}
	}
	
	//released
	if (reading2 == HIGH && lastReading2 == LOW) {
		if (hold2 == 1) hold2 = 0;
	}
	//Set a software limit on what variable can be
	if(x > maxValue) x = maxValue;
	if(x < minValue) x = minValue;

	lastReading = reading;
	lastReading2 = reading2;

	return x;
}