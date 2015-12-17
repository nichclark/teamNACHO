/*
TITLE("TeamNACHO USB Power Supply");
SUBTITLE("Initialization Fxn");
BY("Adam S. Hastings && Nicholas P. Clark");
DATE("17 December 2015");

This code should be IMPLEMENTED to initialize the main code. It 
sets the scale factor on Timer1, starts serial communication for 
debugging, and Timer0's millis() fxn. The function also initializes
the pins. A pull-up resistor is used for Vfeedback pin to help make
the highs high and the lows low. The goal of this was to make the
IMPLEMENTATION of TeamNACHO's Decoding Feedback fxn work better.
*/

#include "inits.h"
#include <arduino.h>
#include <avr/io.h>

#define Vfeedback 6 //pin D6
#define PBa 7 //pin D7
#define PBb 8 //pin D8
#define varyOutput  9 //pin D9
#define constOutput  10 //pin D10
//pin A4 = DAT for 7segDisplay
//pin A5 = CLK for 7segDisplay

void initializations(){
	//TCCR1B is the internal clock the program uses to drive the outputs 
	//being used by the program. The default scale factor for this clock 
	//is 0x04, making the clock frequency 490.20 Hz. By changing the
	//scale factor to 0x01, the new clock frequency is 31372.55 Hz. This 
	//change in clock speed had to be done so that the output pin, pin6, 
	//would be at 31.37kHz. This information is from
	//http://playground.arduino.cc/Main/TimerPWMCheatsheet
	TCCR1B = (TCCR1B & 0b11111000) | 0x01;
	
	//Baud rate of serial monitor used for debugging
	Serial.begin(9600);
	
	millis();

	pinMode(Vfeedback,INPUT_PULLUP); // feedback pin
	pinMode(PBa,INPUT); //PB1
	pinMode(PBb,INPUT); //PB2
	pinMode(varyOutput,OUTPUT); //Flyback gate PWM
	pinMode(constOutput,OUTPUT); //Constant gate PWM
	
	
}
