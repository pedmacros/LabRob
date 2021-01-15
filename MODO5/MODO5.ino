#define baudrate 38400
#define pi 3.141516
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

// Variables para interrupcion motor 1 Derecha
volatile unsigned tiempoUltimaInterrupcionD=0;
volatile unsigned incrementoTiempoTicksD=0;
float frecuenciaD=0;
int WD=0;

//Variables para interrupcion motor 2 Izquierda
volatile unsigned tiempoUltimaInterrupcionI=0;
volatile unsigned incrementoTiempoTicksI=0;
float frecuenciaI=0;
int WI=0;
double W;

int N=8; //Numero de ranuras que tiene el encoder (lo he puesto al azar)
int cont=0;
//int referenciaVel=200;

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

int TiempoCont;
int TicksI,TicksD;


void interrupcionD()
{
  TicksD++;
  /*incrementoTiempoTicksD= tiempo-tiempoUltimaInterrupcionD; //millis comienza la cuenta del tiempo que lleva desde que se inicio el programa
  tiempoUltimaInterrupcionD=tiempo;
  frecuenciaD=(1000.0)/(double)incrementoTiempoTicksD; //1000 porque son milisengundos
  WD=((((2*pi)/N)*frecuenciaD))/48;*/
  
  
}

void interrupcionI()
{
  TicksI++;
  /*incrementoTiempoTicksI= tiempo-tiempoUltimaInterrupcionI; //millis comienza la cuenta del tiempo que lleva desde que se inicio el programa
  tiempoUltimaInterrupcionI=tiempo;
  frecuenciaI=(1000)/(double)incrementoTiempoTicksI;
  WI=((((2*pi)/N)*frecuenciaI))/48;*/
  
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
Kp = 1;
Kd = 2;

KpDif = 0.5;
KdDif = 0;

err = 0;
ref = 30;
U = 0;
err_dif=0;

tiempo=millis();

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
  
 //Lectura de ultrasonidos;
 int DD = ping(Trigger1,Echo1);
 int DI = ping(Trigger2,Echo2);

 UD = 120;
 UI = 120;
 adelante();
  
 //Calculo de velocidad angular media
  W=(WD+WI)/2;
  /*
   //Error velocidad angular
   errPrev=err;
   err=ref-W;

  //Calculo de señales de control
 U = abs(Kp * err + Kd*(err-errPrev)/elapsedTime);
 UDif = abs(KpDif*err + KdDif*(err_dif-errPrevDif)/elapsedTime);

 U+=power;
 UD = U+UDif/2;
 UI = U-UDif/2;
 if(err>1){
   adelante();
 }

 //Para dar escalones cuando el error sea pequeño
 if(err<1 && cont==0){
  ref=ref+20;
  cont=1;
 }
 if(err<1 && cont==1){
  ref=ref-10;
  cont=2;
 }
*/

 char str[100];
 sprintf(str,"%d;%d;%d;%d;%d;0;0;%d;%d\n",Tm,DD,DI,WI,WD,UI,UD);
 Serial3.print(str);
 Serial.println(str);
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
