#include "display.h"
#include <arduino.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();

void displayValue (int value){
	matrix.print(value,DEC);
	//matrix.writeDigitNum(1,true);
	matrix.blinkRate(0);
	matrix.drawColon(true);
	matrix.writeDisplay();
}

#define button2 7
#define button 8

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