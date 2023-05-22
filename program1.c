#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router
 
const char* ssid = "IOT"; //--> Your wifi name or SSID.
const char* password = "password"; //--> Your wifi password.
 
 
//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------
 
WiFiClientSecure client; //--> Create a WiFiClientSecure object.
 
String GAS_ID = "AKfycbwmcOGFpqr0RZvnZmxjss0PRmfWuQ7TltVM-BierNcfVISCFXNi9k9OpMtntKYaztV6"; //--> spreadsheet script ID

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
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(500);

     pinMode(16, INPUT);
     pinMode(A0,INPUT);
     pinMode(5, OUTPUT);
     pinMode(4, OUTPUT);
     pinMode(0, OUTPUT); 
     pinMode(2, OUTPUT); 
     pinMode(13,INPUT);
 
    //--> Start reading DHT11 sensors
  delay(500);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board
 
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------
 
  client.setInsecure();
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
    delay(70);        
    noTone(buzzer);     
    delay(70);
    tone(buzzer, 2000); 
    delay(70);        
    noTone(buzzer);     
    delay(70);  
    tone(buzzer, 2000); 
    delay(70);        
    noTone(buzzer);     
    delay(70); 
    tone(buzzer, 2000); 
    delay(70);        
    noTone(buzzer);     
    delay(70);
  }
  
 }
 
 delay(100);
  
  
  
  
  int h = IR;
  int a = alcohol;
  int s = vspeed;
  
  sendData(a, h,s); //--> Calls the sendData Subroutine
}
 
// Subroutine for sending data to Google Sheets
void sendData(int a, int h, int s) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------
 
  //----------------------------------------Processing data and sending data
  String string_alcohol;
  if (a==1){
    string_alcohol = "False";
  }
  else{
    string_alcohol = "True";
  }
  String string_helmet;
  if (h==1){
     string_helmet = "False";
  }
  else{
     string_helmet = "True";
  }
  
  String string_speed =  String(s);
  String url = "/macros/s/" + GAS_ID + "/exec?Alcohol=" + string_alcohol + "&Helmet=" + string_helmet+"&Speed=" + string_speed+"&Location=9.893821,78.176255";
  Serial.print("requesting URL: ");
  Serial.println(url);
 
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");
 
  Serial.println("request sent");
  //----------------------------------------
 
  //----------------------------------------Checking whether the data was sent successfully or not
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
}