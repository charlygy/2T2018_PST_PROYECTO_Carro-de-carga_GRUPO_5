#include <SoftwareSerial.h> 
SoftwareSerial ModBluetooth(2, 3); // RX | TX 
int AnalogPin = 2;   // Sensor conectado a Analog 0
int LEDpin = 5;      // LED conectado a Pin 6 (PWM)
int ResRead;         // La Lectura de la Resistencia por División de Tensión
int BrilloLED;


const int pinPWMA = 6;
const int pinAIN2 = 7;
const int pinAIN1 = 8;
const int pinBIN1 = 9;
const int pinBIN2 = 10;
const int pinPWMB = 11;
const int pinSTBY = 12;
const int pinLed = 4;
 
const int pinMotorA[3] = { pinPWMA, pinAIN2, pinAIN1 };
const int pinMotorB[3] = { pinPWMB, pinBIN1, pinBIN2 };
 
enum moveDirection {forward,backward};

void setup(){
   pinMode(pinAIN2, OUTPUT);
   pinMode(pinAIN1, OUTPUT);
   pinMode(pinPWMA, OUTPUT);
   pinMode(pinBIN1, OUTPUT);
   pinMode(pinBIN2, OUTPUT);
   pinMode(pinPWMB, OUTPUT);
   pinMode(pinLed, OUTPUT); 
   digitalWrite(pinLed, LOW);  
   ModBluetooth.begin(9600); 
   Serial.begin(9600);  
   ModBluetooth.println("MODULO CONECTADO");  
   ModBluetooth.print("#");  
}
 
void loop(){
  enableMotors();
  //Sensor de peso
  ResRead = analogRead(AnalogPin); // La Resistencia es igual a la lectura del sensor (Analog 0)
  Serial.print("Lectura Analogica = ");
  Serial.println(ResRead);  
  BrilloLED = map(ResRead, 0, 1023, 0, 255);
  // Cambiar el rango de la lectura analógica (0-1023)
  // Utilizamos en analogWrite 8 bits (0-255) configurados en el map
  analogWrite(LEDpin, BrilloLED);
  ModBluetooth.print("Peso Actual"+String("  ")+String(ResRead)+String("  ")+"gramos");
  ModBluetooth.print("#");
  delay(500); //Cien “ms” de espera en cada lectura

  //Bluethoot
  while (ModBluetooth.available()){ 
        char VarChar; 
        VarChar = ModBluetooth.read(); 
                 
        if(VarChar == '0'){ 
          digitalWrite(pinLed, HIGH); 
          delay(100); 
          ModBluetooth.print("RETROCEDIENDO"); 
          Serial.print("LED ENCENDIDO");
          ModBluetooth.print("#");
          move(backward, 80); 
        }
        if(VarChar == '1') { 
          digitalWrite(pinLed, HIGH); 
          delay(100); 
          ModBluetooth.print("AVANZANDO"); 
          Serial.print("LED ENCENDIDO"); 
          ModBluetooth.print("#");
          move(forward, 80);
        }
        if(VarChar == '2') { 
          digitalWrite(pinLed, LOW); 
          delay(100); 
          ModBluetooth.print("DETENIDO"); 
          Serial.print("LED APAGADO"); 
          ModBluetooth.print("#");
          fullStop();
        }
        if(VarChar == '3') { 
          digitalWrite(pinLed, LOW); 
          delay(100); 
          ModBluetooth.print("DERECHA"); 
          Serial.print("LED ENCENDIDO"); 
          ModBluetooth.print("#");
          turn(forward,40);
        }
        if(VarChar == '4') { 
          digitalWrite(pinLed, LOW); 
          delay(100); 
          ModBluetooth.print("IZQUIERDA"); 
          Serial.print("LED ENCENDIDO"); 
          ModBluetooth.print("#");
          turn(backward,40);
        } 
    }
}
 
//Funciones que controlan el vehiculo
void move(int direction, int speed)
{
   if (direction == forward)
   {
      moveMotorForward(pinMotorA, speed);
      moveMotorForward(pinMotorB, speed);
   }
   else
   {
      moveMotorBackward(pinMotorA, speed);
      moveMotorBackward(pinMotorB, speed);
   }
}
 
void turn(int direction, int speed)
{
   if (direction == forward)
   {
      moveMotorForward(pinMotorA, speed);
      moveMotorBackward(pinMotorB, speed);
   }
   else
   {
      moveMotorBackward(pinMotorA, speed);
      moveMotorForward(pinMotorB, speed);
   }
}
 
void fullStop()
{
   disableMotors();
   stopMotor(pinMotorA);
   stopMotor(pinMotorB);
} 
//Funciones que controlan los motores
void moveMotorForward(const int pinMotor[3], int speed)
{
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], HIGH);
   analogWrite(pinMotor[0], speed);
}
 
void moveMotorBackward(const int pinMotor[3], int speed)
{
   digitalWrite(pinMotor[1], HIGH);
   digitalWrite(pinMotor[2], LOW);
   analogWrite(pinMotor[0], speed);
}
 
void stopMotor(const int pinMotor[3]){
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], LOW);
   analogWrite(pinMotor[0], 0);
}
 
void enableMotors(){
   digitalWrite(pinSTBY, HIGH);
}
 
void disableMotors(){
   digitalWrite(pinSTBY, LOW);
}


