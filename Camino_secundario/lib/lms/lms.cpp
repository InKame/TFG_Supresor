#include "lms.h"


// CONVERSIÓN INT16 -> FLOAT NORMALIZADO
//
// Convierte: -32768 ... +32767
// a aproximadamente: -1.0 ... +1.0
static float int16_a_float(int16_t muestra) {
    return (float)muestra / 32768.0f;
}


// FILTRO ADAPTATIVO S(z)
// 
// Calcula: z_hat(n) = S(z) * y(n)
// S(z) tiene 64 coeficientes.
static float filtro_adaptativo(const float *s_z, const int16_t *y_buffer, uint32_t n) {
    float z_hat = 0.0f;
    for (int i = 0; i < LMS_FILTER_LEN; i++) {
        if (n >= (uint32_t)i) {
            float y_n_i = int16_a_float(y_buffer[n - i]);
            z_hat += s_z[i] * y_n_i;
        }
    }
    return z_hat;
}


// ERROR DE IDENTIFICACIÓN
// 
// e_s(n) = z(n) - z_hat(n)
//
// z(n)     : micrófono
// z_hat(n) : modelo S(z)
static float calcular_error(int16_t z_muestra, float z_hat) {
    float z_n = int16_a_float(z_muestra);
    return z_n - z_hat;
}


// ACTUALIZACIÓN LMS
// 
// S_i(n+1) = S_i(n) + mu * e_s(n) * y(n-i)
static void actualizar_coeficientes(float *s_z, float error_n,
    const int16_t *y_buffer, uint32_t n) {
    for (int i = 0; i < LMS_FILTER_LEN; i++) {
        if (n >= (uint32_t)i) {
            float y_n_i = int16_a_float(y_buffer[n - i]);
            s_z[i] += LMS_MU * error_n * y_n_i;
        }
    }
}


// FUNCIÓN PRINCIPAL
void calcular_camino_secundario(const int16_t *y_buffer, const int16_t *z_buffer,
    uint32_t num_muestras, float *s_buffer) {


    // Verificación de punteros
    if (y_buffer == nullptr || z_buffer == nullptr || s_buffer == nullptr ||
        num_muestras == 0) {
        return;
    }


    // Inicializar los 64 coeficientes
    for (int i = 0; i < LMS_FILTER_LEN; i++) {
        s_buffer[i] = 0.0f;
    }


    // LMS
    for (int epoch = 0; epoch < LMS_EPOCHS; epoch++) {
        for (uint32_t n = 0; n < num_muestras; n++) {

            // Salida estimada por S(z)
            float z_hat = filtro_adaptativo(s_buffer, y_buffer, n);


            // Error de identificación
            float error_n = calcular_error(z_buffer[n], z_hat);


            // Adaptación de S(z)
            actualizar_coeficientes(s_buffer, error_n, y_buffer, n);
        }
    }
}