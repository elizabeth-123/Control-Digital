// ==========================================================
// Control PID de velocidad para motor con visualización HMI STONE
// Descripción: Este sistema usa un controlador PID para regular la
// velocidad de un motor de acuerdo a una referencia establecida
// desde una interfaz HMI. 
// ==========================================================

// === LIBRERÍAS ===
#include <Controllino.h>            // Librería para el mapeo de pines del Controllino
#include "Stone_HMI_Define.h"      // Definiciones para la pantalla HMI STONE
#include "Procesar_HMI.h"          // Funciones para comunicación con el HMI

// === Pines y Variables ===
const int pin_motor = CONTROLLINO_D0;     // Salida PWM al motor
const int pin_encoder = CONTROLLINO_IN1;  // Entrada de pulsos del encoder.

volatile unsigned long conteo_pulsos = 0; // Pulsos acumulados del encoder en un periodo
float rpm = 0;                            // Velocidad actual del motor en RPM
const uint16_t PULSOS_POR_REV = 36;      // Pulsos del encoder por revolución completa

// === Temporización ===
const float fs = 20.0;         // Frecuencia de muestreo (Hz)
const float Ts = 1.0 / fs;     // Periodo de muestreo (s)

// === Variables del PID ===
float e[3] = {0, 0, 0};    // e[0]=error actual, e[1]=anterior, e[2]=anteanterior
float u = 0;               // Acumulador de la señal de control
float Kp = 0.3;            // Ganancia proporcional
float Ki = 0.00015;        // Ganancia integral
float Kd = 0.0;            // Ganancia derivativa

float Ti = Kp / Ki;        // Tiempo integral
float Td = 0.0;            // Tiempo derivativo 

float referencia = 0;      // Valor deseado de RPM
float u_k = 0;             // Señal de control limitada (0 a 255)

// === Variables para HMI ===
char label2_text[10];  // Texto de referencia
char label4_text[10];  // Texto de RPM real
char label7_text[10];  // Texto de Kp
char label9_text[10];  // Texto de Ki
char label11_text[10]; // Texto de Kd

// === Temporizadores ===
unsigned long t_previo = 0;   // Para actualización HMI cada 100 ms
unsigned long t_previo1 = 0;  // Para lectura de referencia cada 10 ms

// === PROTOTIPO ISR ===
void contarPulso();  // Interrupción para el encoder

// ==========================================================
// SETUP: Inicialización del sistema
// ==========================================================
void setup() {
  Serial.begin(115200);     // Monitor serial para depuración
  Serial2.begin(115200);    // Comunicación con la pantalla HMI

  // Inicializar gráficas en la pantalla HMI
  STONE_push_series("line_series", "line_series1", 0); // Línea de referencia
  STONE_push_series("line_series", "line_series2", 0); // Línea de RPM real
  STONE_push_series("line_series", "line_series3", 0); // Línea de señal de control

  // Reiniciar el valor del slider de referencia en el HMI
  Stone_HMI_Set_Value("slider", "slider1", NULL, 0);

  // Configurar pines
  pinMode(pin_encoder, INPUT);
  pinMode(pin_motor, OUTPUT);

  // Configurar interrupción externa para contar los pulsos del encoder
  attachInterrupt(digitalPinToInterrupt(pin_encoder), contarPulso, FALLING);

  // === CONFIGURACIÓN DEL TIMER1 PARA INTERRUPCIÓN CADA Ts ===
  noInterrupts();       // Desactiva interrupciones globales
  TCCR1A = 0;           // Timer1 en modo normal
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);     // Modo CTC (Clear Timer on Compare)
  TCCR1B |= (1 << CS12);      // Prescaler 256
  OCR1A = 62500 / fs;         // Valor del comparador para 20 Hz (50 ms)
  TIMSK1 |= (1 << OCIE1A);    // Habilita interrupción por comparación
  interrupts();         // Reactiva interrupciones globales

  HMI_init();           // Inicializa la comunicación con el HMI
}

// ==========================================================
// LOOP PRINCIPAL: solo atiende interfaz HMI y visualización
// ==========================================================
void loop() {
  // === Leer referencia desde HMI cada 10 ms ===
  if (millis() - t_previo1 >= 10) {
    referencia = HMI_get_value("slider", "slider1");
    t_previo1 = millis();
  }

  // === Actualizar etiquetas y gráficas cada 100 ms ===
  if (millis() - t_previo >= 100) {
    t_previo = millis();

    // Conversión a texto para mostrar en etiquetas
    dtostrf(referencia, 7, 2, label2_text);
    dtostrf(rpm, 7, 2, label4_text);
    dtostrf(Kp, 7, 2, label7_text);
    dtostrf(Ki, 7, 2, label9_text);
    dtostrf(Kd, 7, 2, label11_text);

    // Actualización de la HMI con valores actuales
    Stone_HMI_Set_Text("label", "label2", label2_text);
    Stone_HMI_Set_Text("label", "label4", label4_text);
    Stone_HMI_Set_Text("label", "label7", label7_text);
    Stone_HMI_Set_Text("label", "label9", label9_text);
    Stone_HMI_Set_Text("label", "label11", label11_text);

    // Envío de datos a las gráficas del HMI
    STONE_push_series("line_series", "line_series1", referencia);
    STONE_push_series("line_series", "line_series2", rpm);
    STONE_push_series("line_series", "line_series3", u_k);
  }
}

// ==========================================================
// INTERRUPCIÓN DE TIMER1: se ejecuta cada 50 ms (20 Hz)
// - Calcula la velocidad actual
// - Actualiza el PID
// - Envía señal PWM al motor
// ==========================================================
ISR(TIMER1_COMPA_vect) {
  TCNT1 = 0; // Reiniciar contador

  // Calcular RPM con los pulsos del encoder
  float rpm_actual = (float(conteo_pulsos) * 60.0 * fs) / PULSOS_POR_REV;
  conteo_pulsos = 0;  // Reiniciar conteo
  rpm = rpm_actual;   // Guardar velocidad actual

  // === Controlador PID incremental ===
  e[2] = e[1];            // Desplazar errores anteriores
  e[1] = e[0];
  e[0] = referencia - rpm_actual;  // Error actual

  // Cálculo PID incremental (sin derivada)
  float delta_u =
    Kp * (e[0] - e[1]) +         // Componente proporcional incremental
    (Kp * Ts / Ti) * e[0];       // Componente integral

  u = u + delta_u;               // Acumulación de la señal de control

  // Limitar la señal entre 0 y 255 para el PWM
  u_k = constrain(u, 0, 255);
  analogWrite(pin_motor, int(u_k)); // Enviar señal PWM al motor

  // Mostrar información por consola
  Serial.print("\nRPM: ");
  Serial.println(rpm);
  Serial.print("Señal de control: ");
  Serial.println(u_k);
}

// ==========================================================
// INTERRUPCIÓN DEL ENCODER
// Contador que se incrementa en cada flanco de bajada
// ==========================================================
void contarPulso() {
  conteo_pulsos++;  // Incrementa el número de pulsos recibidos
}
