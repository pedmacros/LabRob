// Motor 1
 int ENA=10;
 int IN1=9;
 int IN2=8;

// Motor 2
  int ENB=5;
  int IN3=7;
  int IN4=6;

void setup() {
  // put your setup code here, to run once:
pinMode(ENA,OUTPUT);
pinMode(IN1,OUTPUT);
pinMode(IN2,OUTPUT);
pinMode(ENB,OUTPUT);
pinMode(IN3,OUTPUT);
pinMode(IN4,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  adelante();
  delay(3000);
  atras();
  delay(3000);
  para();
  delay(10000);
}

void adelante ()
{
  //direccion motor 1
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, 255);
  //direccion motor 2
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,255);
}

void atras ()
{
  //direccion motor 1
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, 128);
  //direccion motor 2
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,128);
}

void derecha ()
{
  //direccion motor 1
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, 200);
  //direccion motor 2
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,100);
}

void izquierda ()
{
  //direccion motor 1
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, 50);
  //direccion motor 2
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,150);
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
