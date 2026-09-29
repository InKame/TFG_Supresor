#ifndef PRUEBA_AUDIO_H
#define PRUEBA_AUDIO_H

#include <Arduino.h>

// CONFIGURACIÓN DEL DIEZMADO 
#define DECIMATION_FACTOR 10 


// PROCESAR BLOQUE DEL MICROFONO
void prueba_procesar_bloque(int16_t *audioBlock, uint32_t numSamples);


// VERIFICAR FINALIZACIÓN
bool prueba_finalizada();


// INICIAR PRUEBA
void prueba_iniciar();

#endif