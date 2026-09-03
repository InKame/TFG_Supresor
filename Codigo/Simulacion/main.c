#include <stdio.h>

#include "fxlms.h"
#include "archivo_wav.h"


// ---------------------------------------------------------
// Buffers del algoritmo Fx-LMS
// ---------------------------------------------------------
float x_buf[FILTER_LEN]      = {0.0f};
float y_buf[FILTER_LEN]      = {0.0f};
float x_n_prima[FILTER_LEN]  = {0.0f};

//float w_z[FILTER_LEN] = {
//    0.0f, 0.0f, 0.0f, 0.0f, 
//    0.0f, 0.0f, 0.0f, 0.0f, 
//    0.0f, 0.0f, 0.0f, 0.0f, 
//    0.0f, 0.0f, 0.0f, 0.0f
//};

//float s_z[SEC_LEN] = {
//    0.0200f, 0.0800f, 0.1500f, 0.1000f,
//    0.0500f, 0.0200f, -0.0200f, -0.0400f, 
//    -0.0300f, -0.0100f, 0.0100f, 0.0200f,
//    0.0100f, -0.0100f, -0.0050f, 0.0000f
//};

//float s_techo[SEC_LEN] = {
//    0.0180f, 0.0750f, 0.1400f, 0.1050f,
//    0.0450f, 0.0250f, -0.0150f, -0.0350f,
//    -0.0250f, -0.0150f, 0.0080f, 0.0180f,
//    0.0120f, -0.0080f, -0.0040f, 0.0020f
//};

float w_z[FILTER_LEN] = {0.0f};
float s_z[FILTER_LEN] = {0.9f};
float s_techo[FILTER_LEN] = {0.9f};

// ---------------------------------------------------------
// Parámetros de simulación
// ---------------------------------------------------------
#define MUESTRAS 500000
#define DISTANCIA_M 0.3f
#define MU 0.001f


int main(void) {
    float x_n;
    float y_n;
    float y_n_prima;
    float d_n;
    float e_n;
    float x_s;

    if (!iniciar_lectura_wav("ruido.wav")) {
        printf("Error al abrir ruido.wav\n");
        return 1;
    }

    if (!iniciar_escritura_wav("error.wav")) {
        printf("Error al crear error.wav\n");
        cerrar_wav();
        return 1;
    }


    // -----------------------------------------------------
    // LOOP PRINCIPAL Fx-LMS
    // -----------------------------------------------------

    for (int i = 0; i < MUESTRAS; i++) {
        x_n = leer_muestra_wav();
        buffer(x_n, x_buf);
        y_n = filtro_adaptativo(w_z, x_buf);
        buffer(y_n, y_buf);
        y_n_prima = filtro_adaptativo(s_z, y_buf);
        //d_n = camino_principal(x_n, DISTANCIA_M);
        e_n = error(x_n, y_n_prima);
        escribir_muestra_wav(e_n);
        x_s = filtro_adaptativo(s_techo, x_buf);
        buffer(x_s, x_n_prima);
        coeficientes_w(w_z, e_n, x_n_prima, MU);
    }


    // -----------------------------------------------------
    // Finalización
    // -----------------------------------------------------
    cerrar_wav();
    cerrar_escritura_wav();
    printf("Procesamiento finalizado.\n");
    return 0;
}

//printf("x(n) = %f\n", x_n);
