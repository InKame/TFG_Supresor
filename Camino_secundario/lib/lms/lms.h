#ifndef LMS_H
#define LMS_H

#include <Arduino.h>

// ============================================================
// CONFIGURACIÓN
// ============================================================

#define LMS_FILTER_LEN 256

// Número de pasadas sobre los datos
#define LMS_EPOCHS 10

// Paso de adaptación
#define LMS_MU 0.01f


// ============================================================
// FUNCIÓN PRINCIPAL
// ============================================================
//
// y_buffer:
//     Señal enviada al altavoz.
//
// z_buffer:
//     Señal recibida por el micrófono.
//
// num_muestras:
//     Número de muestras disponibles.
//
// s_buffer:
//     Salida con los 64 coeficientes de S(z).
//
// ============================================================

void calcular_camino_secundario(const int16_t *y_buffer, const int16_t *z_buffer,
    uint32_t num_muestras, float *s_buffer);

#endif