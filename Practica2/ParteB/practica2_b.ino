// ==========================================================
// Control de semáforos con máquina de estados usando Controllino
// Descripción: Simula el comportamiento de dos semáforos coordinados (A y B),
// controlando luces verdes, amarillas y rojas para cada uno, con temporización.
// ==========================================================

#include <Controllino.h>  // Librería específica para placas Controllino

// ==========================================================
// Definición de los estados posibles de la máquina de estados
// - VERDE_A: Vía A en verde, B en rojo
// - AMARILLO_A: Vía A en amarillo, B en rojo
// - VERDE_B: Vía B en verde, A en rojo
// - AMARILLO_B: Vía B en amarillo, A en rojo
// ==========================================================
enum EstadoSemaforo { VERDE_A, AMARILLO_A, VERDE_B, AMARILLO_B };

// ==========================================================
// Estructura Semaforo: agrupa los 3 pines (verde, amarillo, rojo)
// que componen un semáforo para facilitar su acceso y control.
// ==========================================================
struct Semaforo {
  int verde;
  int amarillo;
  int rojo;
};

// ==========================================================
// Definición de los dos semáforos, asignando sus pines de salida
// ==========================================================
Semaforo A = {CONTROLLINO_D0, CONTROLLINO_D1, CONTROLLINO_D2};
Semaforo B = {CONTROLLINO_D6, CONTROLLINO_D7, CONTROLLINO_D8};

// ==========================================================
// Variables de estado y temporización
// - estado: estado actual del sistema
// - t_estado: marca de tiempo del último cambio de estado
// - duracion: duración del estado actual (en ms)
// ==========================================================
EstadoSemaforo estado = VERDE_A;
unsigned long t_estado = 0;
unsigned long duracion = 0;

// ==========================================================
// Constantes para los tiempos de luces en milisegundos
// ==========================================================
const unsigned long TIEMPO_VERDE = 5000;     // Duración del verde (5 segundos)
const unsigned long TIEMPO_AMARILLO = 1000;  // Duración del amarillo (1 segundo)

void setup() {
  // Configura todos los pines de los semáforos como salidas
  pinMode(A.verde, OUTPUT);
  pinMode(A.amarillo, OUTPUT);
  pinMode(A.rojo, OUTPUT);
  pinMode(B.verde, OUTPUT);
  pinMode(B.amarillo, OUTPUT);
  pinMode(B.rojo, OUTPUT);

  // Inicializa el monitor serial (opcional)
  Serial.begin(9600);

  // Se establece el estado inicial del sistema
  establecerEstado(VERDE_A);
}

void loop() {
  // Captura el tiempo actual
  unsigned long t_actual = millis();

  // Si el tiempo del estado actual ha expirado, cambiar al siguiente estado
  if (t_actual - t_estado >= duracion) {
    switch (estado) {
      case VERDE_A:
        // Cambia al estado AMARILLO_A
        establecerEstado(AMARILLO_A);
        digitalWrite(A.verde, HIGH);   // Mantiene verde A
        digitalWrite(B.rojo, HIGH);    // B permanece en rojo
        duracion = TIEMPO_VERDE;       // Duración total del estado
        break;

      case AMARILLO_A:
        // Cambia al estado VERDE_B
        establecerEstado(VERDE_B);
        digitalWrite(A.amarillo, HIGH); // A en amarillo
        digitalWrite(B.rojo, HIGH);     // B sigue en rojo
        duracion = TIEMPO_AMARILLO;
        break;

      case VERDE_B:
        // Cambia al estado AMARILLO_B
        establecerEstado(AMARILLO_B);
        digitalWrite(B.verde, HIGH);   // B en verde
        digitalWrite(A.rojo, HIGH);    // A en rojo
        duracion = TIEMPO_VERDE;
        break;

      case AMARILLO_B:
        // Regresa al estado VERDE_A
        establecerEstado(VERDE_A);
        digitalWrite(B.amarillo, HIGH); // B en amarillo
        digitalWrite(A.rojo, HIGH);     // A sigue en rojo
        duracion = TIEMPO_AMARILLO;
        break;
    }
  }
}

// ==========================================================
// Función: establecerEstado
// Cambia el estado del sistema y apaga todas las luces.
// También actualiza la marca de tiempo de cambio de estado.
// ==========================================================
void establecerEstado(EstadoSemaforo nuevoEstado) {
  estado = nuevoEstado;
  t_estado = millis();  // Marca de tiempo actual

  // Apaga todas las luces de ambos semáforos
  digitalWrite(A.verde, LOW);
  digitalWrite(A.amarillo, LOW);
  digitalWrite(A.rojo, LOW);
  digitalWrite(B.verde, LOW);
  digitalWrite(B.amarillo, LOW);
  digitalWrite(B.rojo, LOW);
}
