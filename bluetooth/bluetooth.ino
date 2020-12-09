#include <SoftwareSerial.h>
#define rxPin 10
#define txPin 11
#define baudrate 38400

String msg;

SoftwareSerial hc05(rxPin ,txPin);

void setup(){
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  
  Serial.begin(9600);
  Serial.println("ENTER AT Commands:");
  hc05.begin(baudrate);
}

void loop(){
  if (hc05.available())
  Serial.write(hc05.read());

  if (Serial.available())
  hc05.write(Serial.read());
  
}
