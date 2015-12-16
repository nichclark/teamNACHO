#include "inits.h"
#include <arduino.h>
#include <avr/io.h>

#define Vfeedback 6 //pin D6
#define PBa 7 //pin D7
#define PBb 8 //pin D8
#define PWMOutput  9 //pin D9
#define ConstOutput  10 //pin D10
//pin A4 = DAT for 7segDisplay
//pin A5 = CLK for 7segDisplay

void initializations(){

	TCCR1B = (TCCR1B & 0b11111000) | 0x01;
	
	//Baud rate of serial monitor used for debugging
	Serial.begin(9600);
	
	millis();

	pinMode(Vfeedback,INPUT_PULLUP); // feedback pin
	pinMode(PBa,INPUT); //PB1
	pinMode(PBb,INPUT); //PB2
	pinMode(PWMOutput,OUTPUT); //Flyback gate PWM
	pinMode(ConstOutput,OUTPUT); //Constant gate PWM
	
	
}
