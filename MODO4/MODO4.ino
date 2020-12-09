#define baudrate 38400

#define speed 128
const int Tm=100;

//Ultrasonido derecha
const int Echo1 = 12;
const int Trigger1 = 13;

//Ultrasonido izquierda
const int Echo2 = 52;   //2 es el sensor de la izquierda
const int Trigger2 = 50;

// Motor 1 Derecha
 int ENA=2;
 int IN1=3;
 int IN2=4;

// Motor 2 izquierda
  int ENB=7;
  int IN3=5;
  int IN4=6;

int state;
  
  
void setup() {
  // put your setup code here, to run once:
  //Ultrasonido
  pinMode(Trigger1,OUTPUT);
  pinMode(Echo1,INPUT);
  pinMode(Trigger2,OUTPUT);
  pinMode(Echo2,INPUT);
//Motores
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  Serial3.begin(baudrate);

  state = 0;
}

void loop() {
  
 //Lectura de ultrasonidos;
 int cm1 = ping(Trigger1,Echo1);
 delay(50);
 int cm2 = ping(Trigger2,Echo2);
 delay(Tm);

 switch(state){
  case 0:
  if(cm1>cm2 && cm1>30 && cm2>30){
      adelante_izquierda();}
  else if(cm2>cm1 && cm2>30 && cm1>30){
      adelante_izquierda;}
  else if(cm1>cm2 && cm1<30 && cm2<30){  
      adelante_derecha;}
  else if(cm2>cm1 && cm2<30 && cm1<30){
      adelante_derecha;}
  else if(cm1>cm2 && cm1>30 && cm2<30){
      adelante_izquierda;}
  else if(cm2>cm1 && cm2>30 && cm1<30){
      adelante_derecha;}
  else{
      adelante();
    }
    if(cm1==30 && cm2 == 30){
      state = 1;
    }
  break;
  
  case 1:
  if(cm1>cm2 && cm1>40 && cm2>40){
      adelante_izquierda();}
  else if(cm2>cm1 && cm2>40 && cm1>40){
      adelante_izquierda;}
  else if(cm1>cm2 && cm1<40 && cm2<40){  
      adelante_derecha;}
  else if(cm2>cm1 && cm2<40 && cm1<40){
      adelante_derecha;}
  else if(cm1>cm2 && cm1>40 && cm2<40){
      adelante_izquierda;}
  else if(cm2>cm1 && cm2>40 && cm1<40){
      adelante_derecha;}
  else{
      adelante();
    }
    if(cm1==40 && cm2 == 40){
      state = 2;
    }
  break;
  case 2:
  adelante();
  delay(1000); //Para que ande un poco recto
  para();
  break;
 }

 
 char str[20];
 sprintf(str,"%d;%d;%d;0;0;0;0\n",Tm,cm1,cm2);
 Serial3.write(str);
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

 void adelante ()
{
  //direccion motor derecha
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speed);
  //direccion motor izquierda
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,speed);
}

void atras ()
{
  //direccion motor 1
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, speed);
  //direccion motor 2
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,speed);
}

void derecha ()
{
  //direccion motor derecha
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speed);
  //direccion motor izquierda
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,speed);
}

void izquierda ()
{
  //direccion motor derecha
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, speed);
  //direccion motor izquierda
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,speed);
}

void adelante_derecha(){
  //direccion motor derecha
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speed-20);
  //direccion motor izquierda
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,speed);
}

void adelante_izquierda(){
  //direccion motor derecha
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speed);
  //direccion motor izquierda
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,speed-20);
}

void atras_derecha(){
  //direccion motor derecho
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, speed);
  //direccion motor izquierdo
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,speed-20);
}

void atras_izquierda(){
  //direccion motor derecho
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, speed-20);
  //direccion motor izquierdo
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,speed);
}

void para ()
{
  //direccion motor derecha
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, 0);
  //direccion motor izquierda
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,0);
}
