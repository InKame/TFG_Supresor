#ifndef WAV_H
#define WAV_H

#include <Arduino.h>

// Guarda un buffer de muestras int16_t como archivo WAV.
// La microSD debe haber sido inicializada previamente.
bool guardarWAV(
    const char *nombreArchivo,
    const int16_t *buffer,
    uint32_t numeroMuestras,
    uint32_t frecuenciaMuestreo
);

#endif