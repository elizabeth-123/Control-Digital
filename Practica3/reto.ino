#include <Controllino.h>
#include "Stone_HMI_Define.h"
#include "Procesar_HMI.h"

const int led1 = CONTROLLINO_D0 ; // LED 1 en salida digital D0
const int led2 = CONTROLLINO_D1; // LED 2 en salida digital D1

const int button1 = CONTROLLINO_I16; // Botón físico para LED 1
const int button2 = CONTROLLINO_I17; // Botón físico para LED 2

bool led1_enabled = false;
bool led2_enabled = false;

int pwmValue1 = 0, pwmValue2 = 0;
float dutyCyclePercent1 = 0, dutyCyclePercent2 = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  HMI_init();  // Inicializar comunicación con HMI

  // Inicializar los spin boxes en 0%
  Stone_HMI_Set_Value("spin_box", "spin_box1", NULL, 0);
  Stone_HMI_Set_Value("spin_box", "spin_box2", NULL, 0);
}

void loop() {
  // Leer valores de SpinBoxes del HMI
  dutyCyclePercent1 = HMI_get_value("spin_box", "spin_box1");
  dutyCyclePercent2 = HMI_get_value("spin_box", "spin_box2");

  // Mapear a rango PWM (0-255)
  if (dutyCyclePercent1 >= 0 && dutyCyclePercent1 <= 100)
    pwmValue1 = map(dutyCyclePercent1, 0, 100, 0, 255);

  if (dutyCyclePercent2 >= 0 && dutyCyclePercent2 <= 100)
    pwmValue2 = map(dutyCyclePercent2, 0, 100, 0, 255);

  // Detectar cambios en botones físicos (con flanco de subida)
  static bool lastButton1State = LOW;
  static bool lastButton2State = LOW;

  bool currentButton1State = digitalRead(button1);
  bool currentButton2State = digitalRead(button2);

  if (currentButton1State == HIGH && lastButton1State == LOW) {
    led1_enabled = !led1_enabled;  // Alternar estado de encendido LED1
    Serial.print("LED1 estado: ");
    Serial.println(led1_enabled ? "ENCENDIDO" : "APAGADO");
  }

  if (currentButton2State == HIGH && lastButton2State == LOW) {
    led2_enabled = !led2_enabled;  // Alternar estado de encendido LED2
    Serial.print("LED2 estado: ");
    Serial.println(led2_enabled ? "ENCENDIDO" : "APAGADO");
  }

  lastButton1State = currentButton1State;
  lastButton2State = currentButton2State;

  // Controlar brillo si LED está habilitado
  if (led1_enabled) {
    analogWrite(led1, pwmValue1);
  } else {
    analogWrite(led1, 0);  // Apagar LED1
  }

  if (led2_enabled) {
    analogWrite(led2, pwmValue2);
  } else {
    analogWrite(led2, 0);  // Apagar LED2
  }

  delay(50); // Pequeño delay para evitar rebotes
}

