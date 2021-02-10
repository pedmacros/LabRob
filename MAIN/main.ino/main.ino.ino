#define baudrate 38400
#define power 110
#define pi 3.141516
#define b 0.1 //metros
#define r 0.0325 //metros

//Motor 1 derecha
#define ENA 2
#define IN1 3
#define IN2 4

//Motor 2 izquierda
#define ENB 7
#define IN3 5
#define IN4 6

//Ultrasonido derecha
int Echo1 = 12;
int Trigger1 = 13;
int DD;

//Ultrasonudo izquierda
int Echo2 = 52;
int Trigger2 = 50;
int DI;

//Errores
float err = 0;
float errPrev = 0;
float err_I=0,err_1_I=0,err_2_I=0,err_D=0,err_1_D=0,err_2_D=0;

//Control MODO1
int Kp_1 = 1;
int Kd_1 = 2;

//Control MODO6
float Kp_6 = 2;
float Kd_6 = 2;
float Ki_6 = 0;
//float q0_6,q1_6,q2_6;

//Control MODO7
float Kp_7 = 0.5;
float Td_6 = 50;
float Ti_6 = 50;
float q0_6,q1_6,q2_6;

//tiempo
int tiempo, TiempoCont, tiempoPrev, elapsedTime;

//Variables para el control en velocidad
float WD = 0; //velocidades en rad/s
float WI = 0;
int N = 8; //Numero de ranuras del encoder
int TicksI,TicksD;
float phi_p=0, phi=0;

char input[4];
int ref = 0;
int ref_I, ref_D;
int vel,i,modo;
int U = 0, UI = 0, UD = 0, U_D = 0, U_I = 0, U_1_I, U_1_D = 0;

void setup() {
  
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

  //Interrupciones
  attachInterrupt(digitalPinToInterrupt(20),interrupcionD, FALLING);  //interrupcion por flanco de bajada
  attachInterrupt(digitalPinToInterrupt(21),interrupcionI, FALLING); 

  vel=20;
  
  Serial3.begin(baudrate); 
  i = 0;

}

void loop() {

   // Medimos el tiempo transcurrido
   tiempoPrev = tiempo;
   tiempo = millis();
   elapsedTime = (tiempo - tiempoPrev);

   //Lectura de ultrasonidos;
   DD = ping(Trigger1,Echo1);
   DI = ping(Trigger2,Echo2);

  if((millis()-TiempoCont)>10)
  {
    //Calculamos la velocidad de la rueda izquierda
    //((Ticks/10.0)*((1000.0/8.0)/48.0)*(2.0*pi))
    WI=TicksI*1.6362;
    WD=TicksD*1.6362;
    phi_p = abs(WD*r - WI*r)/b;
    phi = phi + phi_p*0.01;
    TicksI=0;
    TicksD=0;
    TiempoCont=millis();
  }
   
  //Espera a que el puerto esté libre para escribir las lecturas en el vector input[]
  if(Serial3.available()){
    input[i] = Serial3.read();
    if(input[i] == '.'){
      read_cmd();
      i=0;
    }else {
      i++; 
    }
  }

  switch(modo){
    case 0:
      modo0();
    break;
    case 1:
      modo1();
    break;
    case 6:
      modo6();
    break;
    case 7:
      modo7();
    break;
    default:
      para();
    break;
  }
  
}

void read_cmd(){
  switch(input[0]){
    case 'M':
      modo = input[1]-'0';
      err = 0;
      errPrev = 0;
      err_I=0;
      err_1_I=0;
      err_2_I=0;
      err_D=0;
      err_1_D=0;
      err_2_D=0;
      U_1_D = 0;
      U_1_I = 0;
      break;
    case 'r':
      ref = 10*(input[1]-'0')+(input[2]-'0');
      break;
    case 'v':
      vel = 10*(input[1]-'0')+(input[2]-'0');
      Serial3.println(vel);
      break;
  }
  //Limpia el vector después de leerlo
  for(int j = 0; j<4;j++){
    input[j] = '.';
  }
}

void modo0(){
  para();
}

void modo1(){
 errPrev = err;
 err = (DD+DI)/2 - ref; 
 
 U = abs(Kp_1 * err + Kd_1*(err-errPrev)/elapsedTime);
 U+=power;
 UD = U;
 UI = U;
 if(err<-1)
 {
  atras();
 }else if(err>1){
  adelante();
 }else{
  para();
 }
}

void modo6(){
  //--------------------------------------
  //CONTROL RUEDA DERECHA
  //--------------------------------------
  //Error velocidad angular
  ref_D=20;
  ref_I=20;
  err_D=WD-ref_D;
  
  //Actualización de las señal de control-
  U_D = abs(Kp_6*err_D + Kd_6*(err_D-err_1_D)/elapsedTime);

  err_1_D = err_D;
  U_1_D = U_D;

  UD = U_D+power;
  //--------------------------------------
  //CONTROL RUEDA IZQUIERDA
  //--------------------------------------
  //Error velocidad angular
  err_I=WI-ref_I;

  //Actualización de la señal de control--
  U_I = abs(Kp_6*err_I + Kd_6*(err_I-err_1_I)/elapsedTime);
  
  //Actualización de errores

  err_1_I = err_I;
  U_1_I = U_I;

  UI = U_I+power;

  //Saturación de la señal de control
  if(UD > 255) UD = 255;
  if(UD < 0) UD = 0;
  if(UI > 255) UI = 255;
  if(UI < 0) UI = 0;

 adelante();

}

void modo7(){
  //--------------------------------------
  //CONTROL RUEDA DERECHA
  //--------------------------------------
  //Error velocidad angular
  ref_D=20;
  ref_I=20;
  err_D=WD-ref_D;
  
  //Actualización de las señal de control-
  U_D = abs(Kp_6*err_D + Kd_6*(err_D-err_1_D)/elapsedTime);

  err_1_D = err_D;
  U_1_D = U_D;

  UD = U_D+power;
  //--------------------------------------
  //CONTROL RUEDA IZQUIERDA
  //--------------------------------------
  //Error velocidad angular
  err_I=WI-ref_I;

  //Actualización de la señal de control--
  U_I = abs(Kp_6*err_I + Kd_6*(err_I-err_1_I)/elapsedTime);
  
  //Actualización de errores

  err_1_I = err_I;
  U_1_I = U_I;

  UI = U_I+power;

  //Saturación de la señal de control
  if(UD > 255) UD = 255;
  if(UD < 0) UD = 0;
  if(UI > 255) UI = 255;
  if(UI < 0) UI = 0;
  Serial3.print("phi: ");
  Serial3.println(phi);
 derecha();

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
  analogWrite(ENA, U);
  //direccion motor 2
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,U);
}

void derecha ()
{
  //direccion motor derecha
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, UD);
  //direccion motor izquierda
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,UI);
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

void interrupcionD()
{
  TicksD++;
}

void interrupcionI()
{
  TicksI++;
}
