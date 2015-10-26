// PID Loop Program


int PWMOutput = 6;
int pot = A0;
int kp = 1;
int ki = 2;
int PTerm, ITerm, SumTerm;
int Error, NewError;
int PrevError = 0;
int Desired, Actual;
int DutyCycle;
long SampleTime = 200; //miliseconds
long CurrentTime, LastTime;
int feedback = A1;

void setup() {
  TCCR0B = TCCR0B & B11111000 | B00000001;     // Prescale factor of 1 to get a Fpwm = 62.5kHz
  Serial.begin(9600);
  millis();
  //pinMode(pot,INPUT);
  //pinMode(PWMOutput,OUTPUT);
}

void loop() {
  
  CurrentTime = millis()/64;
  
  if(CurrentTime >= LastTime + SampleTime){
    Desired = analogRead(pot);    //pot value between 0 and 1024
    Actual = analogRead(feedback); //feedback from output voltage
    Error = Desired - Actual;   //calculate the error
  
    PTerm = Error * kp;   //multiply error by proportional term
    NewError = ki * Error;
    ITerm = NewError - PrevError;
    SumTerm = PTerm + ITerm;
    
    PrevError = NewError;

    LastTime = CurrentTime;
    
    DutyCycle = map(SumTerm, 0, 1024, 0, 255);
    Serial.println(DutyCycle);
   // Serial.println(Error);
   // Serial.println(NewError);

  }
  
  analogWrite(PWMOutput, DutyCycle);

}
