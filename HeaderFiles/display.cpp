#include "display.h"
#include "arduino.h"
#include "Wire.h"
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