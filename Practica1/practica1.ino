// ===================================================================
// Proyecto: Secuencia de LEDs en espiral con Controllino
// Descripción: Este programa enciende una secuencia de 9 LEDs
// conectados al Controllino, simulando un patrón en espiral.
// ===================================================================

// Se incluye la librería específica para usar el hardware Controllino
#include <Controllino.h>

// Variables de control de tiempo
unsigned long t_previo = 0;      // Almacena el último instante en que se actualizó un LED (en ms)
unsigned long intervalo = 500;   // Tiempo de espera entre cambios de LED (500 milisegundos)

// ========================================================
// Arreglo de pines digitales del Controllino conectados a LEDs
// El orden no representa la secuencia, solo la conexión física
// ========================================================
int leds[9] = {
  CONTROLLINO_D0,   // LED 0
  CONTROLLINO_D1,   // LED 1
  CONTROLLINO_D2,   // LED 2
  CONTROLLINO_D6,   // LED 3
  CONTROLLINO_D7,   // LED 4
  CONTROLLINO_D8,   // LED 5
  CONTROLLINO_D12,  // LED 6
  CONTROLLINO_D13,  // LED 7
  CONTROLLINO_D14   // LED 8
};

// ========================================================
// Arreglo que define la secuencia de encendido en forma de espiral
// Cada valor representa un índice del arreglo "leds"
// ========================================================
int secuencia[9] = {0, 3, 6, 7, 8, 5, 2, 1, 4};

// ========================================================
// Punteros para acceder a los arreglos "leds" y "secuencia"
// Esto permite manipular los datos usando aritmética de punteros
// ========================================================
int* ptrLeds = leds;
int* ptrSecuencia = secuencia;

// Índice actual dentro de la secuencia
int indice = 0;

void setup() {
  // Configura cada pin de LED como salida y los apaga
  for (int i = 0; i < 9; i++) {
    pinMode(*(ptrLeds + i), OUTPUT);       // Configura como salida el pin apuntado por el puntero
    digitalWrite(*(ptrLeds + i), LOW);     // Asegura que el LED esté apagado al inicio
  }

  // Inicializa el monitor serial para depuración (opcional)
  Serial.begin(9600);
}

void loop() {
  // Lee el tiempo actual en milisegundos desde el inicio del programa
  unsigned long t_actual = millis();

  // Comprueba si ha transcurrido el intervalo establecido (500 ms)
  if (t_actual - t_previo >= intervalo) {
    // Actualiza el tiempo del último cambio
    t_previo = t_actual;

    // Apaga todos los LEDs antes de encender el siguiente
    for (int i = 0; i < 9; i++) {
      digitalWrite(*(ptrLeds + i), LOW);   // Apaga cada LED
    }

    // Obtiene el pin que corresponde al LED de la posición actual en la secuencia
    int pinActual = *(ptrLeds + *(ptrSecuencia + indice));

    // Enciende el LED correspondiente
    digitalWrite(pinActual, HIGH);

    // Avanza al siguiente LED en la secuencia (regresa a 0 si llega al final)
    indice = (indice + 1) % 9;
  }
}
