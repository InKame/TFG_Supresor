// Modulo //
#include "fxlms.h"

// Funcion del buffer de la entrada x(n).
void buffer(float x_n, float x_buf[FILTER_LEN]) {
    int i;

    // Desplazar las muestras anteriores
    for (i = FILTER_LEN - 1; i > 0; i--) {
        x_buf[i] = x_buf[i - 1];
    }

    // Agregar la nueva muestra
    x_buf[0] = x_n;
}

// Funcion del filtro adaptativo W(z).
float filtro_adaptativo(float *w_z, float *x_buf) {
    int i;
    float y_n = 0.0f;

    // y(n) = W(z) * x(n)
    for (i = 0; i < FILTER_LEN; i++) {
        y_n += w_z[i] * x_buf[i];
    }
    return y_n;
}

// Funcion del camino principal real P(z).
float camino_principal(float x_n, float distancia_m) {
    static float delay_buffer[MAX_DELAY] = {0.0f};
    static int index = 0;

    int delay_samples;
    int output_index;
    float d_n;

    // Calcular retardo debido a la distancia
    delay_samples = (int)((distancia_m / VELOCIDAD_SONIDO) * FS);

    // Índice de la muestra que sale del buffer
    output_index = (index - delay_samples + MAX_DELAY) % MAX_DELAY;

    // Señal retardada
    d_n = delay_buffer[output_index];

    // Guardar nueva muestra
    delay_buffer[index] = x_n;

    // Avanzar índice circular
    index++;

    if (index >= MAX_DELAY) {
        index = 0;
    }

    return d_n;
}

// Funcion del valor de salida de error e(n).
float error(float d_n, float y_n_prima) {
    float e = d_n + y_n_prima;
    return e;
}

// Funcion de proximos coeficientes del filtro W(z).
void coeficientes_w(float *w_z, float e, float *x_n_prima, float mu) {
    int i;

    // w_i(n+1) = w_i(n) + mu * e(n) * x'(n-i)
    for (i = 0; i < FILTER_LEN; i++) {
        w_z[i] -= mu * e * x_n_prima[i];
    }
}
