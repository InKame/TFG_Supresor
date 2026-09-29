#ifndef GUARDAR_COEFICIENTES_H
#define GUARDAR_COEFICIENTES_H

#include <Arduino.h>
#include <SD.h>

// Guarda un buffer de coeficientes en formato C/C++.
//
// Ejemplo de salida:
//
// float s_techo[FILTER_LEN] = {
//     0.82f, 0.14f, 0.04f, -0.02f,
//     ...
// };
//
bool guardar_coeficientes(const char *nombreArchivo, const float *coeficientes,
    uint16_t cantidad);

#endif