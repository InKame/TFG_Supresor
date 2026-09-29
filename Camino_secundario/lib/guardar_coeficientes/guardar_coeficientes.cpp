#include "guardar_coeficientes.h"


bool guardar_coeficientes(const char *nombreArchivo, const float *coeficientes,
    uint16_t cantidad) {


    // Verificar punteros
    if (nombreArchivo == nullptr || coeficientes == nullptr ||
        cantidad == 0) {
        return false;
    }


    // Abrir archivo
    File archivo = SD.open(nombreArchivo, FILE_WRITE);
    if (!archivo) {
        return false;
    }


    // Escribir declaración del arreglo
    archivo.print("float s_techo[FILTER_LEN] = {\n");


    // Escribir coeficientes
    for (uint16_t i = 0; i < cantidad; i++) {
        archivo.print(coeficientes[i], 8);
        archivo.print("f");

        // Coma después de cada coeficiente
        if (i < cantidad - 1) {
            archivo.print(", ");
        }

        // 8 coeficientes por línea
        if ((i + 1) % 8 == 0) {
            archivo.print("\n");
        }
    }


    // Cerrar arreglo
    archivo.print("};\n");


    // Cerrar archivo
    archivo.close();

    return true;
}