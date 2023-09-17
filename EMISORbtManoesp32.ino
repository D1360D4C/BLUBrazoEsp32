#include "BluetoothSerial.h"
const int an[] = {36, 39, 34, 35, 32, 33};    //Pins analógicos en los que se han conectado los sensores
int maxim[]={1000, 1000, 1000, 1000, 1000, 1000};    //Vector con los maximos de cada sensor 
int minim[]={10, 10, 10, 10, 10, 10};    //Vector con los minimos de cada sensor
int senval[6];  

BluetoothSerial BT;
String clientName = "ESP32";
bool connected;

void setup() {

while (millis () < 10000)    //Función para la calibración que se ejecutará durante los primeros 10s
  {
    for (int index=0; index <= 5; index++)     //Bucle for para acceder a cada uno de los 5 componentes de los vectores
    {
      senval[index] = analogRead(an[index]);    //Lectura y almacenamiento de los valores de tensión (0-1023) de cada sensor
      if (senval[index] < minim[index])     //Comparación del valor leído con el mínimo
      {
        minim[index]=senval[index];     //Si el valor leido es menor que el mínimo, este se establecerá como nuevo mínimo
      }
      if (senval[index] > maxim[index])     //Comparación del valor leído con el máximo
      {
        maxim[index]=senval[index];     //Si el valor leido es mayor que el máximo, este se establecerá como nuevo máximo
      }
      delay(20);     //Esperar 20 ms entre mediciones
    }
  }
  
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  BT.begin("ESP32_client", true);
  Serial.println("El dispositivo Bluetooth está en modo maestro. Conectando con el anfitrión ...");
  connected = BT.connect(clientName);
  if (connected) {
    Serial.println("¡Conectado exitosamente!");
    digitalWrite(2,HIGH);
  } else {
    while (!BT.connected(5000)) {
      Serial.println("No se pudo conectar. Asegúrese de que el dispositivo remoto esté disponible y dentro del alcance, luego reinicie la aplicación.");
      connected = BT.connect(clientName); 
    }
    
  }
  digitalWrite(2,HIGH);
}

void loop() {
 
int servoposition[6];     //Vector que almacenará las posición (en grados) de los servos
  for (int index=0; index <= 5; index++)     //Bucle for para acceder a cada uno de los 5 componentes de los vectores
  {
    senval[index] = analogRead(an[index]);      //Lectura y almacenamiento de los valores de tensión (0-1023) de cada sensor
    
    servoposition[index] = map(senval[index],minim[index],maxim[index],0,180);     //Transformación de los valores entre 0 y 1023 en la posición en grados del servo (0-180)
    
    servoposition[index] = constrain(servoposition[index],0,180);     //Iguala los valores que estén fuera del intervalo a sus extremos
    BT.write(byte (servoposition[index]));     //Transformación de los valores de servoposition a tipo byte y envío en serie a través de la XBee
  }
  delay(50);     //Esperar 50 ms entre mediciones
  
while (!BT.connected(5000)) {
  digitalWrite(2,LOW);
      Serial.println("No se pudo conectar. Asegúrese de que el dispositivo remoto esté disponible y dentro del alcance, luego reinicie la aplicación.");
      connected = BT.connect(clientName);  
    }
    digitalWrite(2,HIGH);
}