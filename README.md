
# 🛠️ Prácticas de Automatización y Control con Tableros Didácticos

Este repositorio contiene la documentación, códigos fuente y recursos relacionados con las prácticas realizadas utilizando los tableros didácticos basados en **Controllino Mega**, **ESP32 PLC 14** y la interfaz **HMI STONE**, siguiendo la guía oficial proporcionada por el Departamento de Ingeniería Eléctrica, Electrónica y Telecomunicaciones.


![Tablero_componentes](tablero.png)

## 📚 **Contenido del repositorio**

Este repositorio está organizado de acuerdo con las prácticas detalladas en la guía:

- ✅ **Práctica 1:** Manejo de Salidas Digitales con Controllino Mega.
- ✅ **Práctica 2:** Control básico de salidas digitales y aplicación avanzada con Máquina de Estados Finita (FSM).
- ✅ **Práctica 3:** Diseño de interfaz gráfica para control de salidas con HMI STONE.
- ✅ **Práctica 4:** Adquisición de datos de motor, visualización gráfica y control de velocidad con controlador PID.

Cada práctica incluye:
- Descripción y objetivo.
- Códigos fuente comentados.
- Capturas de pantalla o videos de evidencia.
- Documentos en formato IEEE si aplica.
- Archivos de proyecto de HMI (STONE Designer GUI).

## 📂 **Estructura del repositorio**

```
Practicas-Tableros/
├── Practica1/
│   ├── Practica1.ino
│   ├── Evidencias/
│   ├── Informe_Practica1.pdf
│   └── README.md
├── Practica2/
│   ├── ParteA/
│   ├── ParteB/
│   ├── Diagrama_FSM.png
│   ├── Informe_Practica2.pdf
│   └── README.md
├── Practica3/
│   ├── Practica3.ino
│   ├── Proyecto_StoneDesigner/
│   ├── Evidencias/
│   ├── Informe_Practica3.pdf
│   └── README.md
├── Practica4/
│   ├── Practica4.ino
│   ├── Proyecto_StoneDesigner/
│   ├── Controlador_PID/
│   ├── Evidencias/
│   ├── Informe_Practica4.pdf
│   └── README.md
├── Guia_de_uso_de_Tableros.pdf
└── README.md
```

## ⚙️ **Requisitos**

- Arduino IDE configurado para **Controllino Mega** y **ESP32 PLC 14**.
- Librerías necesarias:
  - [Controllino.h](https://www.controllino.com/board-library-setup-in-arduino-ide/)
  - [Procesar_HMI.h](https://github.com/HenryM19/HMI-Stone.git)
- Software **STONE Designer GUI** para diseñar interfaces gráficas en el HMI STONE.

## 🚀 **Cómo usar este repositorio**

1. **Clonar o descargar**
   ```bash
   git clone https://github.com/tu-usuario/Practicas-Tableros.git
   ```

2. **Abrir cada práctica**
   - Navega a la carpeta de la práctica que deseas revisar.
   - Abre el archivo `.ino` en Arduino IDE.
   - Si aplica, abre el proyecto de HMI en STONE Designer GUI.

3. **Cargar el código al PLC**
   - Conecta tu tablero.
   - Sigue los pasos de encendido y carga de firmware según la guía.

4. **Documentar resultados**
   - Agrega tus observaciones, capturas y videos a la carpeta `Evidencias` de cada práctica.

## 📑 **Referencias**

- [Guía de uso de Tableros (PDF)](./Guía_de_uso_de_Tableros.pdf)
- [Controllino Official Website](https://www.controllino.com/)
- [STONE HMI Official Website](https://www.stoneitech.com/)

## ✏️ **Autor**

- **Nombre:** Elizabeth Corte y Dayana Jara
- **Institución:** Departamento de Ingeniería Eléctrica, Electrónica y Telecomunicaciones
- **Fecha:** Junio 2025




