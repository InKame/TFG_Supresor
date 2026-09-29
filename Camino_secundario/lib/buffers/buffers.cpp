#include "buffers.h"


// BUFFERS
int16_t bufferRuido[TOTAL_MUESTRAS];
DMAMEM int16_t bufferMicrofono[TOTAL_MUESTRAS];


// CONTADORES
uint32_t muestrasMicrofono = 0;


// INICIALIZAR BUFFERS
void buffers_iniciar() {
    muestrasMicrofono = 0;
}


// VERIFICAR SI LOS BUFFERS ESTÁN COMPLETOS
bool buffers_completos() {
    return (muestrasMicrofono >= TOTAL_MUESTRAS);
}