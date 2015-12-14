long pwm_value;
long sum;
long avg;
long total;
long final[20];
int i;
int j;
int maxx = 0;

void setup() {
 Serial.begin(9600);
 pinMode(3, INPUT_PULLUP);
 for (i=0; i<20; i++){
  final[i] = 0;
 }
}

void loop() {
  
  for(i=0; i<20; i++){
    do
    {
      pwm_value = pulseIn(3, LOW);
    } while(pwm_value < 10);

    final[i] = pwm_value;

    if (i==19){
      for (j=0; j<20; j++){
        if(final[j] > maxx){
          maxx = final[j];
        }
     }
    }  
  }
  Serial.println(maxx);
  maxx=0;
}  
