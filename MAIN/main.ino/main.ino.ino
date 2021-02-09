#define baudrate 38400
#define power 110

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

//Ultrasonudo izquierda
int Echo2 = 52;
int Trigger2 = 50;

//Errores
int err = 0;
int errPrev = 0;

//Control proporcional MODO1
int Kp_1 = 1;
int Kd_1 = 2;

//tiempo
int tiempo;
int tiempoPrev;
int elapsedTime;

//Ultrasonidos

int DD,DI;

char input[4];
int ref = 0;
int vel,i,modo;
int U = 0;

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
    default:
      para();
    break;
  }
  
}

void read_cmd(){
  switch(input[0]){
    case 'M':
      modo = input[1]-'0';
      break;
    case 'r':
      ref = 10*(input[1]-'0')+(input[2]-'0');
      break;
    case 'v':
      vel = 10*(input[1]-'0')+(input[2]-'0');
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
 if(err<-1)
 {
  atras();
 }else if(err>1){
  adelante();
 }else{
  para();
 }
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
