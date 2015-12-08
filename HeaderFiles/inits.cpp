#include "inits.h"
#include <arduino.h>
#include <avr/io.h>

void initializations(int SampleTime){
	
	TCCR0A = 0;
	TCCR0B = 0;
	TCNT1 = 0; //Set counter to 0
	TCCR0A |= (1 << WGM00) | (1 << WGM01) ; //Phase-correct PWM
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1); //None-inverted mode (High at bottom, Low on match)
	TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00); //No pre-scaler
		
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0; //Set counter to 0
	TCCR1B |= (1 << WGM12); //Set the interrupt in CTC mode so that OCR1A is the top
	OCR1A = 16000000/256/1000*SampleTime; //Create an interrupt at the desired sample time
	TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10); //No pre-scaler
	TIMSK1 |= (1 << OCIE1A); //Enable the interrupt
	

	//Baud rate of serial monitor used for debugging
	Serial.begin(9600);
	
	pinMode(4,INPUT);
	pinMode(5,OUTPUT); 
	pinMode(6,OUTPUT); 
	pinMode(7,INPUT);
	pinMode(8,INPUT);
	pinMode(A4,OUTPUT);
	pinMode(A5,OUTPUT);  
	
}
