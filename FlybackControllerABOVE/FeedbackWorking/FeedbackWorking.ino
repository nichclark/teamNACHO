long pwm_value;
long sum;
long avg;

void setup() {
 Serial.begin(9600);
 pinMode(3, INPUT);
 }

void loop() {
  pwm_value = pulseIn(3, LOW);
  if(pulseIn(3, LOW) > 100){
    for(int i=0; i<10; i++){
      sum += pulseIn(3, LOW); 
    }
  }
  avg = sum/10;
  if(avg > 0){
  sum=0;
  Serial.println(avg);
  }
}
