#include "userinput.h"
#include <arduino.h>
#include <avr/io.h>

#define button2 7
#define button 8
#define bounceTime 50
#define holdTime 32000
#define maxValue 125
#define minValue 5

int userInput (){
	int x;

	int lastReading = LOW;
	int lastReading2 = LOW;

	int hold = 0;
	int hold2 = 0;

	int single = 0;
	int single2 = 0;

	long onTime = 0;
	long lastSwitchTime = 0;
	
	int reading = digitalRead(button);
	int reading2 = digitalRead(button2);
	
	//first pressed
	if (reading == HIGH && lastReading == LOW && reading2 == LOW) {
		onTime = millis();
		x=x+1;
	}
	
	//held
	if (reading == HIGH && lastReading == HIGH && reading2 == LOW) {
		if ((millis() - onTime) > holdTime) {
			x=x+1;
			hold = 1;
		}
	}
	
	//released
	if (reading == LOW && lastReading == HIGH) {
		if (hold == 1) hold = 0;
	}
	
	lastReading = reading;
	
	//DECREMENT SETUP
	
	
	//first pressed
	if (reading2 == HIGH && lastReading2 == LOW && reading == LOW) {
		onTime = millis();
		x=x-1;
	}
	
	//held
	if (reading2 == HIGH && lastReading2 == HIGH && reading == LOW) {
		if ((millis() - onTime) > holdTime) {
			x=x-1;
			hold2 = 1;
		}
	}
	
	//released
	if (reading2 == LOW && lastReading2 == HIGH) {
		if (hold2 == 1) hold2 = 0;
	}
	if(x > maxValue) x = maxValue;
	if(x < minValue) x = minValue;
	lastReading2 = reading2;
	x = map(x,minValue,maxValue,0,1024);
	return x;
}