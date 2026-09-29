#ifndef BUFFERS_H
#define BUFFERS_H

#include <Arduino.h>


// ============================================================
// CONFIGURACIÓN
// ============================================================

// Frecuencia nativa del Audio Library
#define SAMPLE_RATE_NATIVE    44100

// Factor de diezmado
#define DECIMATION_FACTOR     10

// Frecuencia efectiva de los buffers
#define SAMPLE_RATE           (SAMPLE_RATE_NATIVE / DECIMATION_FACTOR)

// Duración de la prueba
#define DURACION_SEGUNDOS     5

// Número de muestras almacenadas a 4410 Hz durante 5 segundos
#define TOTAL_MUESTRAS        (SAMPLE_RATE * DURACION_SEGUNDOS)


// ============================================================
// BUFFERS
// ============================================================

// Buffer del ruido generado
extern int16_t bufferRuido[TOTAL_MUESTRAS];

// Buffer de la señal registrada por el micrófono
extern int16_t bufferMicrofono[TOTAL_MUESTRAS];


// ============================================================
// CONTADORES
// ============================================================

// Número de muestras actualmente almacenadas
extern uint32_t muestrasMicrofono;


// ============================================================
// FUNCIONES
// ============================================================

// Inicializa los buffers y contadores
void buffers_iniciar();

// Devuelve si el buffer del micrófono está lleno
bool buffers_completos();

#endif