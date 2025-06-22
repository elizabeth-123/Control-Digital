#include <Controllino.h>

enum EstadoSemaforo { VERDE_A, AMARILLO_A, VERDE_B, AMARILLO_B };

struct Semaforo {
  int verde;
  int amarillo;
  int rojo;
};

Semaforo A = {CONTROLLINO_D0, CONTROLLINO_D1, CONTROLLINO_D2};
Semaforo B = {CONTROLLINO_D6, CONTROLLINO_D7, CONTROLLINO_D8};

EstadoSemaforo estado = VERDE_A;
unsigned long t_estado = 0;
unsigned long duracion = 0;

// Constantes para los tiempos
const unsigned long TIEMPO_VERDE = 5000;  // 5 segundos
const unsigned long TIEMPO_AMARILLO = 2000; // 2 segundos

void setup() {
  pinMode(A.verde, OUTPUT);
  pinMode(A.amarillo, OUTPUT);
  pinMode(A.rojo, OUTPUT);
  pinMode(B.verde, OUTPUT);
  pinMode(B.amarillo, OUTPUT);
  pinMode(B.rojo, OUTPUT);
  Serial.begin(9600); // Inicia comunicación serial para depuración
  establecerEstado(VERDE_A);
}

void loop() {
  unsigned long t_actual = millis();

  if (t_actual - t_estado >= duracion) {
    switch (estado) {
      case VERDE_A:
        establecerEstado(AMARILLO_A);
        break;
      case AMARILLO_A:
        establecerEstado(VERDE_B);
        break;
      case VERDE_B:
        establecerEstado(AMARILLO_B);
        break;
      case AMARILLO_B:
        establecerEstado(VERDE_A);
        break;
    }
  }
}

void establecerEstado(EstadoSemaforo nuevoEstado) {
  estado = nuevoEstado;
  t_estado = millis();

  // Apagar todos
  digitalWrite(A.verde, LOW); digitalWrite(A.amarillo, LOW); digitalWrite(A.rojo, LOW);
  digitalWrite(B.verde, LOW); digitalWrite(B.amarillo, LOW); digitalWrite(B.rojo, LOW);

  switch (estado) {
    case VERDE_A:
      digitalWrite(A.verde, HIGH);
      digitalWrite(B.rojo, HIGH);
      duracion = TIEMPO_VERDE;
      break;
    case AMARILLO_A:
      digitalWrite(A.amarillo, HIGH);
      digitalWrite(B.rojo, HIGH);
      duracion = TIEMPO_AMARILLO;
      break;
    case VERDE_B:
      digitalWrite(B.verde, HIGH);
      digitalWrite(A.rojo, HIGH);
      duracion = TIEMPO_VERDE;
      break;
    case AMARILLO_B:
      digitalWrite(B.amarillo, HIGH);
      digitalWrite(A.rojo, HIGH);
      duracion = TIEMPO_AMARILLO;
      break;
  }
}
