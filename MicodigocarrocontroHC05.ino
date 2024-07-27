#include <SoftwareSerial.h>

// Configuración de pines para la comunicación serial Bluetooth
const int rxPin = 2; // Pin de recepción (RX) del módulo Bluetooth
const int txPin = 3; // Pin de transmisión (TX) del módulo Bluetooth

// Crear un objeto SoftwareSerial para la comunicación Bluetooth
SoftwareSerial bluetoothSerial(rxPin, txPin);

// Variables del Sensor Ultrasónico
const int trigPin = 4;
const int echoPin = 5;

// Motores
const int motorI1 = 6;
const int motorI2 = 7;
const int motorD1 = 8;
const int motorD2 = 9;

// Otros
const int vel = 250;
const int disMin = 7; // Nueva distancia mínima para detenerse en 7 cm
int distancia;

// Variable para almacenar el tiempo en que se detuvo el carro
unsigned long tiempoDetenido = 0; 

void setup() {
  Serial.begin(9600); // Comunicación serial con el monitor serie
  bluetoothSerial.begin(9600); // Comunicación serial con el módulo Bluetooth

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  pinMode(motorI1, OUTPUT);
  pinMode(motorI2, OUTPUT);
  pinMode(motorD1, OUTPUT);
  pinMode(motorD2, OUTPUT);
}

void loop() {
  distancia = calcularDistancia();

  if (bluetoothSerial.available() > 0) {
    char infoDeApp = bluetoothSerial.read();
    switch (infoDeApp) {
      case 'A': // Adelante
        moverAdelante();
        break;
      case 'B': // Atrás
        moverAtras();
        break;
      case 'I': // Izquierda
        girarIzquierda();
        break;
      case 'D': // Derecha
        girarDerecha();
        break;
      case 'P': // Parar
        detener();
        tiempoDetenido = millis(); // Almacenar el tiempo actual
        break;
      default:
        detener(); // Por defecto, detener el carro si el comando no es reconocido
        break;
    }
  }

  // Si han pasado más de 3 segundos desde que se detuvo el carro, 
  // se permite seguir manejando
  if (millis() - tiempoDetenido >= 3000) {
    tiempoDetenido = 0; // Reiniciar el tiempo detenido
  }

  // Si se detecta un obstáculo a 7 cm, se detiene el carro pero se 
  // permite seguir manejando después de 3 segundos
  if (distancia <= disMin && millis() - tiempoDetenido >= 3000) {
    detener();
  }

  delay(20);
}

void moverAdelante() {
  analogWrite(motorI1, vel);
  analogWrite(motorI2, 0);
  analogWrite(motorD1, vel);
  analogWrite(motorD2, 0);
}

void moverAtras() {
  analogWrite(motorI1, 0);
  analogWrite(motorI2, vel);
  analogWrite(motorD1, 0);
  analogWrite(motorD2, vel);
}

void girarIzquierda() {
  analogWrite(motorI1, vel);
  analogWrite(motorI2, 0);
  analogWrite(motorD1, 0);
  analogWrite(motorD2, 0);
}

void girarDerecha() {
  analogWrite(motorI1, 0);
  analogWrite(motorI2, 0);
  analogWrite(motorD1, vel);
  analogWrite(motorD2, 0);
}

void detener() {
  analogWrite(motorI1, 0);
  analogWrite(motorI2, 0);
  analogWrite(motorD1, 0);
  analogWrite(motorD2, 0);
}

int calcularDistancia() {
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Convertir a cm

  return distance;
}
