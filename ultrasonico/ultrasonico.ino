
const int Echo1 = 12;
const int Trigger1 = 13;
const int Echo2 = 52;   //2 es el sensor de la izquierda
const int Trigger2 = 50;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Trigger1,OUTPUT);
  pinMode(Echo1,INPUT);
  pinMode(Trigger2,OUTPUT);
  pinMode(Echo2,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

 int cm1 = ping(Trigger1,Echo1);

 Serial.print("Distancia derecha:");
 Serial.println(cm1);
 delay(50);
  int cm2 = ping(Trigger2,Echo2);
  Serial.print("Distancia izquierda:");
  Serial.println(cm2);
 delay(1000);
}


 int ping(int Trigger1, int Echo1) {
  long duration, distanceCm;
  digitalWrite(Trigger1, LOW); //para generar un pulso limpio se pone a LOW 4us
  delayMicroseconds(4);
  digitalWrite(Trigger1, HIGH); //se genera el disparo de 10 us
  delayMicroseconds(10);
  digitalWrite(Trigger1, LOW);
  duration= pulseIn(Echo1, HIGH);
  distanceCm= duration *10/292/2;
  return distanceCm;
 }
