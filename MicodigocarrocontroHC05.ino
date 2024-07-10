//Carro a Control Remoto y App con AppInventor
//Variables del Sensor Ultrasónico
int trigPin = 3;
int echoPin = 4;

// Motores
int motorI1 = 10;
int motorI2 = 9;
int motorD1 = 6;
int motorD2 = 5;

// Otros
int vel = 250;
int disMin = 7; // Nueva distancia mínima para detenerse en 7 cm
int distancia;

// Variable para almacenar el tiempo en que se detuvo el carro
unsigned long tiempoDetenido = 0; 

void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  pinMode(motorI1, OUTPUT);
  pinMode(motorI2, OUTPUT);
  pinMode(motorD1, OUTPUT);
  pinMode(motorD2, OUTPUT);
}

void loop() {
  distancia = calcularDistancia();

  if (Serial.available() > 0) {
    char infoDeApp = Serial.read();
    if (infoDeApp == 'A') {
      digitalWrite(motorI1, 0);
      digitalWrite(motorI2, vel);
      digitalWrite(motorD1, 0);
      digitalWrite(motorD2, vel);
    } else if (infoDeApp == 'B') {
      digitalWrite(motorI1, vel);
      digitalWrite(motorI2, 0);
      digitalWrite(motorD1, vel);
      digitalWrite(motorD2, 0);
    } else if (infoDeApp == 'I') {
      digitalWrite(motorI1, 0);
      digitalWrite(motorI2, vel);
      digitalWrite(motorD1, 0);
      digitalWrite(motorD2, 0);
    } else if (infoDeApp == 'D') {
      digitalWrite(motorI1, 0);
      digitalWrite(motorI2, 0);
      digitalWrite(motorD1, 0);
      digitalWrite(motorD2, vel);
      
    } else if (infoDeApp == 'P') {
      // Si la tecla se suelta, se detiene el carro y se almacena 
      //el tiempo
      digitalWrite(motorI1, 0);
      digitalWrite(motorI2, 0);
      digitalWrite(motorD1, 0);
      digitalWrite(motorD2, 0);
      // Almacenar el tiempo actual
      tiempoDetenido = millis(); 
    }
  }

  // Si han pasado más de 3 segundos desde que se detuvo el carro, 
  //se permite seguir manejando
  if (millis() - tiempoDetenido >= 3000) {
    // Reiniciar el tiempo detenido
    tiempoDetenido = 0;
  }

  // Si se detecta un obstáculo a 7cm, se detiene el carro pero se 
  //permite seguir manejando después de 3 segundos
  if (distancia <= disMin && millis() - tiempoDetenido >= 3000) {
    digitalWrite(motorI1, 0);
    digitalWrite(motorI2, 0);
    digitalWrite(motorD1, 0);
    digitalWrite(motorD2, 0);
  }

  delay(20);
}

int calcularDistancia() {
  long duration, distance;

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;

  delay(10);
  return distance;
}