#define baudrate 38400

#define power 110
const int Tm=100;

//Ultrasonido derecha
const int Echo1 = 12;
const int Trigger1 = 13;
int DD;

//Ultrasonido izquierda
const int Echo2 = 52;   //2 es el sensor de la izquierda
const int Trigger2 = 50;
int DI;

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

int KpDif;
int KdDif;

int err;
int errPrev;

int err_dif;
int errPrevDif;

int ref;
int U;
int UDif;
int UD;
int UI;

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

KpDif = 0.5;
KdDif = 0;

err = 0;
ref = 30;
U = 0;
err_dif=0;

tiempo=millis();
}

void loop() {

 // Medimos el tiempo transcurrido
 tiempoPrev = tiempo;
 tiempo = millis();
 elapsedTime = (tiempo - tiempoPrev) / 1000;
 
 //Lectura de ultrasonidos;
 DD = ping(Trigger1,Echo1);
 DI = ping(Trigger2,Echo2);

 //Error distancia
 errPrev = err;
 err = (DD+DI)/2 - ref;

 //Error ángulo
 errPrevDif= err_dif;
 err_dif = (DD-DI);
 
 U = abs(Kp * err + Kd*(err-errPrev)/elapsedTime);
 UDif = abs(KpDif*err + KdDif*(err_dif-errPrevDif)/elapsedTime);

 U+=power;
 UD = U+UDif;
 UI = U-UDif;
 if(err<0)
 {
  atras();
 }else if(err>0){
  adelante();
 }else{
  para();
 }
 char str[20];
 sprintf(str,"%d;%d;%d;0;0;0;0\n",elapsedTime,DD,DI);
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
  analogWrite(ENA, UD);
  //direccion motor 2
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,UI);
}

void atras ()
{
  //direccion motor 1
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, UD);
  //direccion motor 2
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,UI);
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
