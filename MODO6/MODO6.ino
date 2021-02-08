#define baudrate 38400
#define pi 3.141516
#define power 110

int Tm;

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

// Variables para interrupcion motor 1 Derecha
float WD=0;

//Variables para interrupcion motor 2 Izquierda
float WI=0;
double W;

int N=8; //Numero de ranuras que tiene el encoder (lo he puesto al azar)
//int referenciaVel=200;

//Control PID
int Kp;
int Td;
int Ti;

int q0,q1,q2;

float err_I,err_1_I,err_2_I,err_D,err_1_D,err_2_D;

float ref_I,ref_D;
int U_I,U_D,U_1_I,U_1_D;
int UD;
int UI;

//Tiempo
int tiempo;
int tiempoPrev;
int elapsedTime;  

int TiempoCont;
int TicksI,TicksD;

int tiempoExp;


void interrupcionD()
{
  TicksD++;
}

void interrupcionI()
{
  TicksI++;
}

  
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
  Serial.begin(baudrate);

   state=0;
//Interrupciones
attachInterrupt(digitalPinToInterrupt(20),interrupcionD, FALLING);  //interrupcion por flanco de bajada
attachInterrupt(digitalPinToInterrupt(21),interrupcionI, FALLING); 

//Inicializar constantes de control
Kp = 0.1;
Td = 100;
Ti = 50;


err_I = 0;
err_1_I = 0;
err_2_I = 0;

err_D = 0;
err_1_D = 0;
err_2_D = 0;


ref_I = 12.0;
ref_D = 12.0;
U_I = 0;
U_1_I=0;
U_D = 0;
U_1_D=0;

tiempo = millis();
tiempoExp = millis();
Tm = millis();
TiempoCont = 0;
}

void loop() {
 // Medimos el tiempo transcurrido
 tiempoPrev = tiempo;
 tiempo = millis();
 elapsedTime = (tiempo - tiempoPrev) / 1000;

 if((millis()-TiempoCont)>10)
 {
  //Calculamos la velocidad de la rueda izquierda
  //((Ticks/10.0)*((1000.0/8.0)/48.0)*(2.0*pi))
  WI=TicksI*1.6362;
  WD=TicksD*1.6362;
  TicksI=0;
  TicksD=0;
  TiempoCont=millis();
 }

  if((millis()-tiempoExp) > 3000 && (millis()-tiempoExp) < 6000)
  {
    ref_D = 30;
    ref_I = 30;
  }else{
    ref_D = 0;
    ref_I = 0;
  }
  
  ref_D = ref_D/3.25;
  ref_I = ref_I/3.25;
 //Lectura de ultrasonidos;
 int DD = ping(Trigger1,Echo1);
 int DI = ping(Trigger2,Echo2);

 //Calculo de velocidad angular media
 W=(WD+WI)/2.0;


 //--------------------------------------
 //CONTROL RUEDA DERECHA
 //--------------------------------------
 //Error velocidad angular
 err_D=WD-ref_D;

 q0 = Kp*(1+(Td/elapsedTime));
 q1 = Kp*(-1-2*Td/elapsedTime + elapsedTime/Ti);
 q2 = Kp*Td/elapsedTime;

 U_D = U_1_D + q0*err_D + q1*err_1_D + q2*err_2_D;

 UD = U_D+power;

 err_2_D = err_1_D;
 err_1_D = err_D;
 U_1_D = U_D;



 //--------------------------------------
 //CONTROL RUEDA IZQUIERDA
 //--------------------------------------
 //Error velocidad angular
 err_I=WI-ref_I;

 q0 = Kp*(1+(Td/elapsedTime));
 q1 = Kp*(-1-2*Td/elapsedTime + elapsedTime/Ti);
 q2 = Kp*Td/elapsedTime;

 U_I = U_1_I + q0*err_I + q1*err_1_I + q2*err_2_I;

 UI = U_I+power;
 
 err_2_I = err_1_I;
 err_1_I = err_I;
 U_1_I = U_I;
 
 if(UD > 255) UD = 255;
 if(UD < 0) UD = 0;
 if(UI > 255) UI = 255;
 if(UI < 0) UI = 0;

 adelante();

 char str[100];
 Tm = millis()-Tm;
 sprintf(str,"%d;%d;%d;%d;%d;%d;%d;%d;%d\n",Tm,DD,DI,(int)WI,(int)WD,(int)ref_I,(int)err_I,UI,UD);
 Serial3.print(str);
 Tm = millis();
 
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
  analogWrite(ENA, UD);
  //direccion motor izquierda
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
  //direccion motor derecha
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, power);
  //direccion motor izquierda
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,power);
}

void izquierda ()
{
  //direccion motor derecha
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, power);
  //direccion motor izquierda
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,power);
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
