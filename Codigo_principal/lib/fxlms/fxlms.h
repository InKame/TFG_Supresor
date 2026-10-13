// Algoritmo Fx-LMS Header //

#ifndef FXLMS_H
#define FXLMS_H

// Includes //
#include <stdio.h>

// Structs //


// Global Vars //
#define FILTER_LEN 128   // Largo del filtro de control W
#define SEC_LEN 4      // Largo del modelo secundario S

#define FS 48000.0f
#define VELOCIDAD_SONIDO 343.0f
#define MAX_DELAY 1000

// Functions //
void buffer(float x_n, float x_buf[FILTER_LEN]);
float filtro_adaptativo(float *w_z, float *x_buf);
float error(float d_n, float y_n_prima);
void coeficientes_w(float *w_z, float e, float *x_n_prima, float mu);

#endif