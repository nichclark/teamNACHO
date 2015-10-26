// PID Loop Program

// ------------------ Pin Set-Up ----------------------//
int PWMOutput = 6;
int VSupply = 8;
int pot = A0;
int feedback = A1;
// ------------------ Declare Constant Variables ---------------------//
int P, I, D, Duty;
int  Last, LastTime;
int Desire, Actual;
int MaxIntegral, Integral;
long To, Tf;
int Scale = 0.25;
//------------------- Declare Adjustable Variables -------------------//
int kP = 1;
int kI = 0;
int kD = 0;
int SampleTime = 200; //miliseconds

void setup() {
  TCCR0B = TCCR0B & B11111000 | B00000001;     // Prescale factor of 1 to get a Fpwm = 62.5kHz
  Serial.begin(9600);
  millis();

}

void loop() {
  int Error = CalculateFlybackError();
  int RunPID = ElapsedTime();
  int Duty = PIDloop(Error, RunPID);
  FlybackPWMSignal(Duty);

}
int CalculateFlybackError(){
  int Desire = analogRead(pot);
  int Actual = analogRead(feedback);
  int Error = Desire - Actual;
  return Error;
}
int ElapsedTime(){
  int RunPID;
  int CurrentTime = millis()/64;
  if(CurrentTime >= LastTime + SampleTime){
    LastTime = CurrentTime;
    RunPID = 1;
  }
  else {
    RunPID = 0;
  }
  return RunPID;
}
void FlybackPWMSignal(int Duty){
  analogWrite (PWMOutput,Duty);
}
int PIDloop(int Error, int Run) {
  if (Run == 0 ){
    return Duty;
  }
  else {
    if (abs(Error) < MaxIntegral){ // prevent integral 'windup'
      Integral = Integral + Error; // accumulate the error integral
    }
    else {
      Integral=0; // zero it if out of bounds
    }
    P = Error * kP; // calc proportional term
    I = Integral * kI; // integral term
    D = (Last - Actual) * kD; // derivative term
    Duty = P + I + D; // Total  = P+I+D
    Duty = Duty * Scale; // scale  to be in the range 0-255
    
    if (abs(Duty)>255) {
      Duty = 255;
    }
    Last = Actual; // save current value for next time
    return Duty;
  }
}
