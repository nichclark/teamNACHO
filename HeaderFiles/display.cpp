#include "display.h"
#include "arduino.h"
#include "Wire.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();

void displayValue (int value, int minValue, int maxValue){
	matrix.begin(0x70);
	int A;int B;int C;int D;
	//value will have a max value of 1250
	//value = map(value,0,1024,minValue,maxValue);
	A = value/1000;
	B = (value/100) - (A*10);
	C = (value/10) - (A*100) - (B*10);
	D = (value/1) - (A*1000) - (B*100) - (C*10);
	
	matrix.writeDigitNum(1,A);
	matrix.writeDigitNum(2,B,true);
	matrix.writeDigitNum(4,C);
	matrix.writeDigitNum(5,D);
	matrix.writeDisplay();
}