// ==========================================================
// Control de brillo PWM de dos LEDs mediante botones físicos 
// y una HMI STONE. Usa Controllino como controlador principal.
// ==========================================================

#include <Controllino.h>         // Librería específica para el hardware Controllino
#include "Stone_HMI_Define.h"    // Librería para definición de objetos HMI STONE
#include "Procesar_HMI.h"        // Librería para procesar la comunicación HMI

// ==========================================================
// Definición de pines de salida para los LEDs
// - led1: conectado a salida digital D0
// - led2: conectado a salida digital D1
// ==========================================================
const int led1 = CONTROLLINO_D0;  // LED 1 en D0
const int led2 = CONTROLLINO_D1;  // LED 2 en D1

// ==========================================================
// Definición de pines de entrada para los botones físicos
// - button1: botón para controlar encendido/apagado de LED 1
// - button2: botón para controlar encendido/apagado de LED 2
// ==========================================================
const int button1 = CONTROLLINO_I16; // Botón físico asociado a LED 1
const int button2 = CONTROLLINO_I17; // Botón físico asociado a LED 2

// ==========================================================
// Variables de estado de los LEDs
// - led1_enabled: indica si LED 1 está habilitado
// - led2_enabled: indica si LED 2 está habilitado
// ==========================================================
bool led1_enabled = false;
bool led2_enabled = false;

// ==========================================================
// Variables para control de brillo PWM
// - pwmValue1, pwmValue2: valores PWM (0-255)
// - dutyCyclePercent1, dutyCyclePercent2: porcentaje de ciclo útil (0-100%)
// ==========================================================
int pwmValue1 = 0, pwmValue2 = 0;
float dutyCyclePercent1 = 0, dutyCyclePercent2 = 0;

void setup() {
  // ==========================================================
  // Inicializa la comunicación serie para depuración y para HMI
  // ==========================================================
  Serial.begin(115200);
  Serial2.begin(115200);

  // Configura los pines de los LEDs como salidas
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Configura los pines de los botones como entradas
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  // Inicializa la comunicación con la HMI STONE
  HMI_init();

  // Inicializa los Spin Boxes de la HMI en 0% de ciclo útil
  Stone_HMI_Set_Value("spin_box", "spin_box1", NULL, 0);
  Stone_HMI_Set_Value("spin_box", "spin_box2", NULL, 0);
}

void loop() {
  // ==========================================================
  // 1) Lectura de valores de ciclo útil desde la HMI
  // ==========================================================
  dutyCyclePercent1 = HMI_get_value("spin_box", "spin_box1");
  dutyCyclePercent2 = HMI_get_value("spin_box", "spin_box2");

  // Mapeo de 0-100% a rango PWM 0-255 para LED 1
  if (dutyCyclePercent1 >= 0 && dutyCyclePercent1 <= 100)
    pwmValue1 = map(dutyCyclePercent1, 0, 100, 0, 255);

  // Mapeo de 0-100% a rango PWM 0-255 para LED 2
  if (dutyCyclePercent2 >= 0 && dutyCyclePercent2 <= 100)
    pwmValue2 = map(dutyCyclePercent2, 0, 100, 0, 255);

  // ==========================================================
  // 2) Lectura de botones físicos con detección de flanco de subida
  // ==========================================================
  static bool lastButton1State = LOW;  // Estado anterior botón 1
  static bool lastButton2State = LOW;  // Estado anterior botón 2

  bool currentButton1State = digitalRead(button1);
  bool currentButton2State = digitalRead(button2);

  // Detecta flanco de subida en botón 1 y alterna estado de LED 1
  if (currentButton1State == HIGH && lastButton1State == LOW) {
    led1_enabled = !led1_enabled;  // Invertir estado
    Serial.print("LED1 estado: ");
    Serial.println(led1_enabled ? "ENCENDIDO" : "APAGADO");
  }

  // Detecta flanco de subida en botón 2 y alterna estado de LED 2
  if (currentButton2State == HIGH && lastButton2State == LOW) {
    led2_enabled = !led2_enabled;  // Invertir estado
    Serial.print("LED2 estado: ");
    Serial.println(led2_enabled ? "ENCENDIDO" : "APAGADO");
  }

  // Actualiza estados anteriores
  lastButton1State = currentButton1State;
  lastButton2State = currentButton2State;

  // ==========================================================
  // 3) Control de brillo PWM de cada LED
  // ==========================================================
  if (led1_enabled) {
    analogWrite(led1, pwmValue1);   // Aplica PWM calculado a LED 1
  } else {
    analogWrite(led1, 0);           // Apaga LED 1 si está deshabilitado
  }

  if (led2_enabled) {
    analogWrite(led2, pwmValue2);   // Aplica PWM calculado a LED 2
  } else {
    analogWrite(led2, 0);           // Apaga LED 2 si está deshabilitado
  }
}
