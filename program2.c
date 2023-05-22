int potentiometer = 0; 
int IR = 0;
int IR2 = 0;
int in1 = 4;         
int in2 = 5;
int ConA = 0;       
int speed1 = 0;
int vspeed = 0;
const int buzzer = 14;
int flag = 0;
int alcohol = 0;
void setup() {
  
 Serial.begin(115200);
 pinMode(16, INPUT);
 pinMode(A0,INPUT);
 pinMode(5, OUTPUT);
 pinMode(4, OUTPUT);
 pinMode(0, OUTPUT); 
 pinMode(2, OUTPUT); 
 pinMode(13,INPUT);
}

void loop() {
 
 digitalWrite(in1, LOW);            
 digitalWrite(in2, HIGH);
 potentiometer = analogRead(A0);
 potentiometer = potentiometer * 0.2492668622;   
 vspeed = potentiometer * 0.3921568627450980392156862745098;
 alcohol = digitalRead(13);       
 IR = digitalRead(16);
 IR2 = digitalRead(12);


 
 Serial.print("Speed = ");
 Serial.println(vspeed);
 Serial.print("Helmet = ");
 Serial.println(IR);
  Serial.print("Alcohol = ");
 Serial.println(alcohol);
 if (IR == 1){
  digitalWrite(2, LOW); 
  analogWrite(ConA, 0); 
  flag = 0;
 }
 else {
  digitalWrite(2,HIGH);
  analogWrite(ConA, potentiometer);
  if (flag ==0){
  tone(buzzer, 1000); 
  delay(500);        
  noTone(buzzer);     
  delay(500);    
  flag = 1;   
  } 
  if(IR2 ==0 && vspeed > 90){
    tone(buzzer, 2000); 
    delay(25);        
    noTone(buzzer);     
    delay(25);   
  }
 }
 
 delay(500);
}