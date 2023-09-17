#include "BluetoothSerial.h"
#include <ESP32Servo.h>    //Importar librería servo
BluetoothSerial BT;
Servo servo[6];     //Vector para el control de cada servo
int serv[]={12, 14, 27, 26, 25, 32};    //Vector con los maximos de cada sensor 

void setup() {
  Serial.begin(115200);
  BT.begin("ESP32");
  Serial.println("El dispositivo Bluetooth está listo para emparejarse");
    
  for(int i=0; i<6; i++)  // i=2 para que servos desde el D2 hasta el D11
  {
   servo[i].attach(serv[i]);     //Servos 0,1,2,3,4,5,6 en los pins 12,14, 27, 26, 25, 32
  }
}

void loop() {

  byte bufer[6];    //Vector en el que almacenar los valores recibidos
  if (BT.available())     //Comprueba si se están recibiendo datos
  {
    BT.readBytes(bufer,6);     //Si se están recibiendo datos, los almacenará de 10 en 10 en el vector bufer
    for (int index = 0; index < 6; index++)    //Bucle for para acceder a cada uno de los 10 componentes de los vectores
    {
     servo[index].write(bufer[index]);    //Indica a cada servo la posición en la que se tiene que colocar
    }
  }
    
  if (BT.available()) {
    char response = BT.read(); // Lee la respuesta del anfitrión
    
    Serial.println(response);
  }
}