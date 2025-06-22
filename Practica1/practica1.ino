#include <Controllino.h>

// Tiempo no bloqueante
unsigned long t_previo = 0;
unsigned long intervalo = 500; // Tiempo entre LEDs (ms)

// Lista de pines de LEDs
int leds[9] = {
  CONTROLLINO_D0, CONTROLLINO_D6, CONTROLLINO_D12,
  CONTROLLINO_D13, CONTROLLINO_D14, CONTROLLINO_D8,
  CONTROLLINO_D2, CONTROLLINO_D1, CONTROLLINO_D7
};

int* puntero = leds; // Puntero al arreglo de LEDs
int indice = 0;       // LED actual

void setup() {
  Serial.begin(9600);

  // Configura todos los LEDs como salida
  for (int i = 0; i < 9; i++) {
    pinMode(*(puntero + i), OUTPUT);
    digitalWrite(*(puntero + i), LOW); // Asegura que todos inicien apagados
  }
}

void loop() {
  unsigned long t_actual = millis();

  if (t_actual - t_previo >= intervalo) {
    t_previo = t_actual;

    // Apaga el LED anterior
    int ledAnterior = *(puntero + indice);
    digitalWrite(ledAnterior, LOW);

    // Avanza al siguiente índice
    indice = (indice + 1) % 9;

    // Enciende el nuevo LED
    int ledActual = *(puntero + indice);
    digitalWrite(ledActual, HIGH);

    // Mensaje opcional para depuración
    Serial.print("LED encendido en pin: ");
    Serial.println(ledActual);
  }
}
