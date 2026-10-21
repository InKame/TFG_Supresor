# Sistema de Control Activo de Ruido mediante Fx-LMS en configuración Feedback

## Introducción

Este repositorio contiene el firmware y los códigos de simulación de un sistema de Control Activo de Ruido (ANC). El sistema se desarrolla como una prueba de concepto orientada a la atenuación del ruido en un puesto de trabajo con niveles superiores a 90 dB, mediante una implementación en tiempo real sobre hardware de bajo coste. El proyecto se divide en tres módulos: simulación del algoritmo, estimación del camino secundario y ejecución del sistema ANC completo.

## Algoritmo

El sistema emplea una arquitectura ANC basado en el algoritmo Fx-LMS de tipo Feedback, como se observa en el siguiente diagrama de bloques. Este algoritmo busca minimizar de forma iterativa el error cuadrático medio mediante la actualización de los coeficientes de un filtro adaptativo. En un sistema de control activo de ruido, el objetivo consiste en reducir la señal de error acústico mediante la generación de una señal de control que interfiera destructivamente con el ruido original.

<!-- Insertar aquí el diagrama de bloques del sistema ANC -->

## Componentes



## Código



## Referencias
