// ==========================================================
// Control de secuencia de LEDs con botones y máquina de estados
// Descripción: Este programa permite controlar el encendido de una
// secuencia de 9 LEDs de forma normal, inversa o apagada, usando
// tres botones conectados a un Controllino.
// ==========================================================

#include <Controllino.h>  // Librería específica del hardware Controllino

// ==========================================================
// Definición de estados posibles del sistema
// - APAGADO: todos los LEDs apagados
// - SECUENCIA_NORMAL: LEDs se encienden de izquierda a derecha
// - SECUENCIA_INVERSA: LEDs se encienden de derecha a izquierda
// ==========================================================
enum Estado { APAGADO, SECUENCIA_NORMAL, SECUENCIA_INVERSA };
Estado estado_actual = APAGADO;  // Estado inicial del sistema

// ==========================================================
// Definición de pines donde se conectan los botones
// - boton1: inicia la secuencia normal
// - boton2: inicia la secuencia inversa
// - boton3: apaga todos los LEDs y reinicia la secuencia
// ==========================================================
const int boton1 = CONTROLLINO_I16;
const int boton2 = CONTROLLINO_I17;
const int boton3 = CONTROLLINO_I18;

// ==========================================================
// Variables para control de tiempo no bloqueante
// - t_previo: almacena el último instante de actualización
// - intervalo: tiempo entre encendido de LEDs (500 ms)
// ==========================================================
unsigned long t_previo = 0;
unsigned long intervalo = 500;

// ==========================================================
// Arreglo con los pines de salida conectados a los LEDs
// El orden define el patrón visual de encendido
// ==========================================================
int leds[9] = {
  CONTROLLINO_D0, CONTROLLINO_D6, CONTROLLINO_D12,
  CONTROLLINO_D13, CONTROLLINO_D14, CONTROLLINO_D8,
  CONTROLLINO_D2, CONTROLLINO_D1, CONTROLLINO_D7
};

// Índice actual dentro de la secuencia de LEDs
int indice = 0;

void setup() {
  Serial.begin(9600);  // Inicializa el puerto serial para depuración (opcional)

  // Configura los pines de botones como entradas digitales
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(boton3, INPUT);

  // Configura todos los pines de LEDs como salidas y los apaga
  for (int i = 0; i < 9; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  // ==========================================================
  // Lectura de botones para cambiar de estado
  // Cada botón asigna un estado diferente a la variable estado_actual
  // ==========================================================
  if (digitalRead(boton1) == HIGH) {
    estado_actual = SECUENCIA_NORMAL;
    indice = 0;  // Reinicia desde el primer LED
  } 
  else if (digitalRead(boton2) == HIGH) {
    estado_actual = SECUENCIA_INVERSA;
    indice = 8;  // Comienza desde el último LED
  } 
  else if (digitalRead(boton3) == HIGH) {
    estado_actual = APAGADO;
    apagarTodosLosLeds();  // Apaga inmediatamente todos los LEDs
  }

  // ==========================================================
  // Máquina de estados: ejecuta la lógica de acuerdo al estado actual
  // ==========================================================
  switch (estado_actual) {
    case APAGADO:
      // Apaga todos los LEDs continuamente para asegurar que estén apagados
      for (int i = 0; i < 9; i++) {
        digitalWrite(leds[i], LOW);
      }
      break;

    case SECUENCIA_NORMAL:
      // Si ha pasado el intervalo, enciende el siguiente LED en orden creciente
      if (millis() - t_previo >= intervalo) {
        t_previo = millis();  // Actualiza el temporizador
        ejecutarSecuenciaNormal();
      }
      break;

    case SECUENCIA_INVERSA:
      // Si ha pasado el intervalo, enciende el siguiente LED en orden decreciente
      if (millis() - t_previo >= intervalo) {
        t_previo = millis();  // Actualiza el temporizador
        ejecutarSecuenciaInversa();
      }
      break;
  }

  delay(10);  // Pequeña pausa para evitar rebotes mecánicos en botones
}

// ==========================================================
// Función: ejecutarSecuenciaNormal
// Enciende un LED a la vez en orden creciente (0 → 8)
// ==========================================================
void ejecutarSecuenciaNormal() {
  apagarTodosLosLeds();                  // Apaga todos los LEDs antes de encender uno
  digitalWrite(leds[indice], HIGH);      // Enciende el LED actual
  indice = (indice + 1) % 9;             // Avanza al siguiente índice circularmente
}

// ==========================================================
// Función: ejecutarSecuenciaInversa
// Enciende un LED a la vez en orden decreciente (8 → 0)
// ==========================================================
void ejecutarSecuenciaInversa() {
  apagarTodosLosLeds();                  // Apaga todos los LEDs
  digitalWrite(leds[indice], HIGH);      // Enciende el LED actual
  indice = (indice - 1 + 9) % 9;         // Retrocede al índice anterior con recorrido circular
}

// ==========================================================
// Función: apagarTodosLosLeds
// Apaga todos los LEDs del arreglo
// ==========================================================
void apagarTodosLosLeds() {
  for (int i = 0; i < 9; i++) {
    digitalWrite(leds[i], LOW);
  }
}
