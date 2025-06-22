#include <Controllino.h>

// Definición de estados
enum Estado { APAGADO, SECUENCIA_NORMAL, SECUENCIA_INVERSA };
Estado estado_actual = APAGADO;

// Pines de botones
const int boton1 = CONTROLLINO_I16; // Botón para secuencia normal
const int boton2 = CONTROLLINO_I17; // Botón para secuencia inversa
const int boton3 = CONTROLLINO_I18; // Botón para reiniciar/apagar

// Tiempo no bloqueante
unsigned long t_previo = 0;
unsigned long intervalo = 500; // Tiempo entre LEDs (ms)

// Lista de pines de LEDs
int leds[9] = {
  CONTROLLINO_D0, CONTROLLINO_D6, CONTROLLINO_D12,
  CONTROLLINO_D13, CONTROLLINO_D14, CONTROLLINO_D8,
  CONTROLLINO_D2, CONTROLLINO_D1, CONTROLLINO_D7
};

int indice = 0;  // Índice actual

void setup() {
  Serial.begin(9600);

  // Configura pines de entrada
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(boton3, INPUT);

  // Configura todos los LEDs como salida y los apaga
  for (int i = 0; i < 9; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  // Lectura de botones
  if (digitalRead(boton1) == HIGH) {
    estado_actual = SECUENCIA_NORMAL;
    indice = 0; // Reinicia el índice
  } else if (digitalRead(boton2) == HIGH) {
    estado_actual = SECUENCIA_INVERSA;
    indice = 8; // Comienza desde el final
  } else if (digitalRead(boton3) == HIGH) {
    estado_actual = APAGADO;

  }

  // Máquina de estados
  switch (estado_actual) {
    case APAGADO:
      apagarTodosLosLeds();
      break;

    case SECUENCIA_NORMAL:
      if (millis() - t_previo >= intervalo) {
        t_previo = millis();
        ejecutarSecuenciaNormal();
      }
      break;

    case SECUENCIA_INVERSA:
      if (millis() - t_previo >= intervalo) {
        t_previo = millis();
        ejecutarSecuenciaInversa();
      }
      break;
  }

  delay(10); // Pequeño retardo para evitar rebotes
}

void ejecutarSecuenciaNormal() {
  apagarTodosLosLeds();
  digitalWrite(leds[indice], HIGH);
  indice = (indice + 1) % 9;
}

void ejecutarSecuenciaInversa() {
  apagarTodosLosLeds();
  digitalWrite(leds[indice], HIGH);
  indice = (indice - 1 + 9) % 9;
}

void apagarTodosLosLeds() {
  for (int i = 0; i < 9; i++) {
    digitalWrite(leds[i], LOW);
  }
}