#define baudrate 38400

#define power 100

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

//Control proporcional
int Kp;
int Kd;
int err;
int errPrev;
int ref;
int U;

//Tiempo
int tiempo;
int tiempoPrev;
int elapsedTime;  
  
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

//Inicializar constantes de control
  Kp = 1;
  Kd = 2;
  err = 0;
  ref = 30;
  U = 0;
}

void loop() {

 // Medimos el tiempo transcurrido
 tiempoPrev = tiempo;
 tiempo = millis();
 elapsedTime = (tiempo - tiempoPrev);
 
 //Lectura de ultrasonidos;
 int cm1 = ping(Trigger1,Echo1);
 int cm2 = ping(Trigger2,Echo2);
 
 errPrev = err;
 err = (cm1+cm2)/2 - ref; 
 
 U = abs(Kp * err + Kd*(err-errPrev)/elapsedTime);
 U+=power;
 if(err<-1)
 {
  atras();
 }else if(err>1){
  adelante();
 }else{
  para();
 }
 char str[20];
 sprintf(str,"%d;%d;%d;%d;%d;0;0\n",elapsedTime,cm1,cm2,ref,(int)U);
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
  //direccion motor 1
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, U);
  //direccion motor 2
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,U);
}

void atras ()
{
  //direccion motor 1
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, U);
  //direccion motor 2
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,U);
}

void derecha ()
{
  //direccion motor 1
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, power);
  //direccion motor 2
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,power);
}

void izquierda ()
{
  //direccion motor 1
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, power);
  //direccion motor 2
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,power);
}

void para ()
{
  //direccion motor 1
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, 0);
  //direccion motor 2
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,0);
}
