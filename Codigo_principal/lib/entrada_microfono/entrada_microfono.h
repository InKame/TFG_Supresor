#ifndef ENTRADA_MICROFONO_H
#define ENTRADA_MICROFONO_H

#include <Arduino.h>
#include <Audio.h>

// Inicializa el sistema de entrada de audio
void entrada_microfono_init();

// Indica cuántos bloques de audio están disponibles
int entrada_microfono_disponible();

// Obtiene el siguiente bloque de audio
// Retorna un puntero a 128 muestras int16_t
int16_t* entrada_microfono_leer();

// Libera el bloque después de utilizarlo
void entrada_microfono_liberar();

#endif