# Sistema de Control Activo de Ruido mediante Fx-LMS en configuración Feedback

## Introducción

Este repositorio contiene el firmware y los códigos de simulación de un sistema de Control Activo de Ruido (ANC). El sistema se desarrolla como una prueba de concepto orientada a la atenuación del ruido en un puesto de trabajo con niveles superiores a 90 dB, mediante una implementación en tiempo real sobre hardware de bajo coste. El proyecto se divide en tres módulos: simulación del algoritmo, estimación del camino secundario y ejecución del sistema ANC completo.

## Algoritmo

El sistema emplea una arquitectura ANC basado en el algoritmo Fx-LMS de tipo Feedback, como se observa en el siguiente diagrama de bloques. Este algoritmo busca minimizar de forma iterativa el error cuadrático medio mediante la actualización de los coeficientes de un filtro adaptativo.

<img width="1008" height="464" alt="BD_Fx_LMS_Feedback" src="https://github.com/user-attachments/assets/6900856b-2a9a-4a2c-918f-b6a73b6333da" />

## Componentes

 - Microcontrolador: Teensy 4.1.

 - Interfaz de audio: Teensy Audio Board Rev D, utilizada para la adquisición y reproducción de señales de audio.

 - Micrófono: módulo MAX4466, conectado a la entrada LINE IN de la interfaz de audio y alimentado desde el pin de 3,3 V del microcontrolador, con tierra común (GND).

 - Altavoz: altavoz comercial conectado a la salida de audio mediante un conector jack de 3,5 mm, con alimentación propia.

 - Alimentación: batería externa conectada al puerto microUSB del Teensy 4.1.

 - Almacenamiento: tarjeta microSD, utilizada según los requerimientos de la implementación.

## Estructura

El repositorio se organiza en las siguientes carpetas:

### Simulacion/

Contiene el código para evaluar el funcionamiento del algoritmo a partir de una señal de ruido previamente grabada. El programa recibe un archivo de audio WAV que representa la señal de ruido d(n) y genera otro archivo WAV con la señal de error e(n).

### Camino_secundario/

Contiene el código para estimar la respuesta impulsional del camino secundario, correspondiente a la trayectoria acústica entre el altavoz y el micrófono. Reproduce una señal de ruido blanco a través del altavoz y registra simultáneamente la señal recibida por el micrófono durante aproximadamente 10 segundos. A partir de ambas señales, calcula los coeficientes que modelan el camino secundario y genera un archivo de texto con estos y un indicador de calidad de la estimación.

### Codigo_principal/

Contiene la implementación completa del sistema ANC en tiempo real. Ejecuta continuamente el algoritmo, adquiriendo las señales de audio, actualizando el filtro de control y generando la señal antirruido mediante el altavoz.

Antes de ejecutar el sistema, es necesario configurar los coeficientes del camino secundario obtenidos en Camino_secundario/ y establecer el factor de convergencia (\mu), de acuerdo con las condiciones de operación y la estabilidad del algoritmo.

## Referencias

[1] Pablo Bernadí and Pablo Gomez. Implementación de un control activo de ruido en
configuración feedback basado en un microcontrolador. Elektron, 2:53–62, 12 2018.
doi:10.37537/rev.elektron.2.2.49.2018.

[2] S. Haykin. Adaptive Filter Theory, volume 4. 01 2002.

[3] Feiran Yang, Yin Cao, Ming Wu, Felix Albu, and Jun Yang. Frequency-domain
filtered-x lms algorithms for active noise control: A review and new insights. Applied
Sciences, 8(11), 2018. URL: https://www.mdpi.com/2076-3417/8/11/2313, doi:
10.3390/app8112313.
