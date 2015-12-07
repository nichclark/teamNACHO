/* TeamNACHO's "NACHO Power" USB Power Supply- CGrade Code
 *  By: Nicholas Clark & Adam Hastings
 *  Date Complete: 07 December 2015
 *  
 *  The following code is for the CGrade Power supply. For the CGrade,
 *  the two sides of the circuit (primary and secondary) are tied
 *  together. As a result, the feedback from the output voltage can
 *  be read through an analog pin. The user sets the output voltage
 *  through the use of a potentiometer, which is also read through
 *  an analog pin. These two values are then used to calculate the
 *  error. The error is then used in a PID loop. Through trial and
 *  error, it was found that a PI loop was all that was needed. For
 *  the CGrade, there was no subfunctions or header files used/created.
 *  This will be done for above and beyond circuit and for the PCB.
 *  
 */
// These variables correspond to pins on the Arduino
int PWMOutput = 6;
int pot = A0;
int feedback = A1;
// These global variables are initialized and set to zero
long LastTime = 0;
int OutputSignal = 0;
int Last = 0;
int Integral = 0;
int DutyCycle = 0;
int Error = 0;
int P = 0;
int I = 0;
int D = 0;
int Duty = 0;
int Actual = 0;
int Desired = 0;

//The follow varables are what get used in the PID loop
int kP = 0.2; //proportional constant
int kI = 1; //integral constant
int kD = 0; //derivative constant
int IntegralRange = 500; //sets when the intergral term start taking effect
long SampleTime = 50; //how often PID loop runs in miliseconds

void setup() {
  //TCCR0B is the internal clock the program uses. The default scale 
  //factor for this clock is 0x04, making the clock frequency 976.5625 Hz.
  //By changing the scale factor to 0x01, the new clock frequency is
  //62500 Hz. This change in clock speed had to be done so that the output
  //pin, pin6, would be at 62.5kHz. This information is from
  //http://playground.arduino.cc/Main/TimerPWMCheatsheet
  TCCR0B = (TCCR0B & 0b11111000) | 0x01;
  Serial.begin(9600); //Baud rate of serial monitor used for debugging
}
  
void loop() {
  //Since clock was changed to 62.5kHz, had to divide clock by 64(0x04) to
  //be able to look at the clock in milliseconds
  long CurrentTime = millis()/64;
  //Reading the user input
  Desired = analogRead(pot);
  //Reading the output voltage 
  Actual = analogRead(feedback); 

  //How oftem the PID loop runs is set by the SampleTime Variable.
  if(CurrentTime - LastTime > SampleTime){
    //Calculate the error in the output voltage
    Error = Desired - Actual;   
    //Determine if the Integral should be used in the PID calculation.
    //The integral is found by adding the new error to the prevous error
    //By having a range on the Integral, this prevents the integral
    //value from getting too large.
    if(abs(Error) < IntegralRange) Integral = Integral + Error;
    else Integral = 0;
    //The terms of the PID loop are calculated below:
    //Multiply error by proportional constant
    P = Error * kP;
    //Multiply the integral term by the integral constant
    I = Integral * kI;
    //Determine the differenece between the current user input value and
    //the last user input. This is then multiplied by the derivative constant
    D = (Last - Actual) * kD;
    //Add the three terms together to determine the output duty cycle
    Duty = P + I + D;
    //A max and min limit were set on the output duty cycle. This was done
    //to protect the harware.
    if(Duty > 1000) Duty = 1000;
    if(Duty < 0) Duty = 0; 
    //The map fxn takes a number between 0 and 1024 and converts it
    //to be between 0 and 255. This is needed as the analogWrite fxn
    //needs a value between 0 and 255.
    DutyCycle = map(Duty, 0, 1024, 0, 255); 
    //Save the current user input to be used to calculate the derivative
    Last = Actual;
    //Save the current time to make sure the PID loop runs at the correct
    //sample time
    LastTime = CurrentTime;
    //The following serial print commands were used for debug. Wanted to see
    //how the PID loop was responding to the error and what the output
    //duty cycle was being set to as a 10bit and 8bit number.
    Serial.print("Error equals ");
    Serial.print(Error);Serial.print(", ");
    Serial.print("Output Duty = ");
    Serial.print(Duty);Serial.print(", ");
    Serial.println(DutyCycle);
  }
  //The duty cycle controls the output of this pin. Since this command is
  //outside of the PID loop, the pin with have a constant duty cycle for 
  //every clock cycle until the PID loop is run. When the PID loop is run,
  //the output duty cycle is changed and will now be constant till the next time.
  analogWrite(PWMOutput, DutyCycle);
}

