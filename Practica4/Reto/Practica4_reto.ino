// === LIBRERIAS ===
#include <Controllino.h>
#include "Stone_HMI_Define.h"
#include "Procesar_HMI.h"

// === Pines y Variables ===
const int pin_motor = CONTROLLINO_D0;
const int pin_encoder = CONTROLLINO_IN1;

volatile unsigned long conteo_pulsos = 0;
float rpm = 0;
const uint16_t PULSOS_POR_REV = 36;
const float fs = 1/0.05;

// Variables PID
float e[3] = {0, 0, 0};
float u = 0;
float Kp = 0.25;
float Ti = 0.15;
float Td = 0.0375;
float rpm_ref = 0;
float pwm = 0;

// HMI
char label2_text[10]; // Referencia
char label4_text[10]; // RPM real

bool emergencia = false;  // NUEVA VARIABLE: Estado de emergencia

unsigned long t_previo = 0;
unsigned long t_previo1 = 0;

// FUNCIONES
void contarPulso();

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  
  STONE_push_series("line_series", "line_series1", 0);
  STONE_push_series("line_series", "line_series2", 0);
  Stone_HMI_Set_Value("slider", "slider1", NULL, 0);
  
  pinMode(pin_encoder, INPUT);
  pinMode(pin_motor, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin_encoder), contarPulso, FALLING);
  
  noInterrupts();
  TCCR1A = 0b00000000;
  TCCR1B = 0b00000100;      // Preescaler 256
  TIMSK1 |= B00000010;      // Habilitar interrupción por comparación
  OCR1A = 62500 / fs;       // Comparador para periodo de muestreo
  interrupts();

  HMI_init();
}

void loop() {
  if (millis() - t_previo1 >= 10) {
    rpm_ref = HMI_get_value("slider", "slider1");  // RPM directa
    rpm_ref = constrain(rpm_ref, 0, 4400);         // Limitar a rango seguro


    t_previo1 = millis();
  }

  if (millis() - t_previo >= 100) {
    t_previo = millis();

    dtostrf(rpm_ref, 7, 2, label2_text);  // RPM deseada
    dtostrf(rpm, 7, 2, label4_text);      // RPM actual

    Stone_HMI_Set_Text("label", "label2", label2_text);
    Stone_HMI_Set_Text("label", "label4", label4_text);

    STONE_push_series("line_series", "line_series1", rpm_ref);
    STONE_push_series("line_series", "line_series2", rpm);
  }
}

// === Interrupción cada periodo de muestreo ===
ISR(TIMER1_COMPA_vect) {
  TCNT1 = 0;

  float rpm_actual = (float(conteo_pulsos) * 60.0 * fs) / PULSOS_POR_REV;
  conteo_pulsos = 0;
  rpm = rpm_actual;

  // === Verificar emergencia ===
  if (HMI_get_value("button", "button1") == 1) {
    analogWrite(pin_motor, 0);   // Apagar motor
    rpm_ref = 0;
    u = 0;                       // Reiniciar control
    e[0] = e[1] = e[2] = 0;
    return;                      // Salir de ISR
  }

  // === Control PID (recurrencia discreta) ===
  e[2] = e[1];
  e[1] = e[0];
  e[0] = rpm_ref - rpm_actual;

  float Ts = 1/fs; // periodo de muestreo
  u += Kp * (e[0] - e[1] + (Ts / Ti) * e[0] + (Td / Ts) * (e[0] - 2 * e[1] + e[2]));

  // Saturación
  pwm = constrain(u, 0, 255);
  analogWrite(pin_motor, int(pwm));
}

// === Contador de pulsos del encoder ===
void contarPulso() {
  conteo_pulsos++;
}
