# 📕 Práctica 4: Adquisición de Datos y Control PID

## 🎯 Objetivo
Adquirir datos de un motor DC, mostrar gráficas en HMI y controlar su velocidad con un PID.

## 📂 Archivos incluidos
- `Practica4.ino`: Código base para adquisición de datos y control.
- `Proyecto_StoneDesigner/`: Proyecto de HMI para sliders, gráficas y labels.
- `Informe_Practica4.pdf`: [Documento explicativo en formato IEEE](/Informe_practicas).
- `Practica4_labview.vi`: Proyecto para adquisicion de datos para hallar la funcion de transferencia del motor.
- `Practica4_matlab.vi`: Codigo base para obtener la funcion de transferencia.
- `Evidencias/`: Fotos, videos y resultados experimentales.
- `Practica4.txt`: Datos adquiridos del motor.

## 🔍 Evidencia

Interfaz final:

<img src="Evidencias/interfaz.jpeg" alt="Interfaz" width="500" />

A continuación se muestra una imagen que muestra como el controlador PID regula automáticamente la velocidad
del motor DC del EPC a un valor de referencia definido.



<img src="Evidencias/practica4_01.jpeg" alt="Resultados 1" width="500" />

<img src="Evidencias/practica4_02.jpeg" alt="Resultados 2" width="500" />


## 🎥 Video de demostración

## Respuesta del PID

<a href="https://drive.google.com/file/d/1Z9c8-Cyg3Enw6BcZSSwLrkEjmazu3bCz/view?usp=sharing">
  <img src="Evidencias/miniatura1.png" alt="Ver video" width="400" />
</a>

## Respuesta a una perturbación

<a href="https://drive.google.com/file/d/1bA8jwb2Lauek65oQo7Ab-Pu2mnWW8pT1/view?usp=sharing">
  <img src="Evidencias/miniatura.png" alt="Ver video" width="400" />
</a>

## 🗒️ Descripción
Se implementa control PWM, conteo de pulsos mediante interrupciones, cálculo de RPM y un PID para regular la velocidad. Se visualizan datos en tiempo real en la HMI.

## 👤 Autor
Elizabeth Corte y Dayana Jara
