#include "inits.h"
#include <arduino.h>
#include <avr/io.h>

void initializations(){
	
	//TCCR0B is the internal clock the program uses. The default scale
	//factor for this clock is 0x04, making the clock frequency 976.5625 Hz.
	//By changing the scale factor to 0x01, the new clock frequency is
	//62500 Hz. This change in clock speed had to be done so that the output
	//pin, pin6, would be at 62.5kHz. This information is from
	//http://playground.arduino.cc/Main/TimerPWMCheatsheet
	TCCR0B = (TCCR0B & 0b11111000) | 0x01;
	//Baud rate of serial monitor used for debugging
	Serial.begin(9600); 
	
}
