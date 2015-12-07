#include "feedback.h"
#include <arduino.h>
#include <avr/io.h>

int calcFeedback(int feedback){

	//Reads a PWM signal's duty cycle and frequency.
	double duty;
	double Vfb;
	long highTime = 0;
	long lowTime = 0;
	long tempPulse;
	
	tempPulse = pulseIn(feedback,HIGH);
	if(tempPulse>highTime){
		highTime = tempPulse;
	}
	tempPulse = pulseIn(feedback,LOW);
	if(tempPulse>lowTime){
		lowTime = tempPulse;
	}
	duty = highTime/(double (lowTime+highTime));

	Vfb = 255*duty;
	Vfb = map(Vfb, 0, 255, 0, 1024);
	
	return (int) Vfb;
}