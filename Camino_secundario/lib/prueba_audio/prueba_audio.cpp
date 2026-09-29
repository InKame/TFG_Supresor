#include "prueba_audio.h"
#include "buffers.h"


// ESTADO
static bool pruebaFinalizada = false;


// CONTADOR DEL DIEZMADO
//
// El Audio Library trabaja a: 44100 Hz
// y entrega bloques de: 128 muestras
//
// Nosotros almacenamos: 1 muestra cada 10
// por lo que los buffers quedan a: 44100 / 10 = 4410 Hz
//
// Este contador debe mantenerse entre bloques.
static uint32_t contadorDiezmado = 0;


// INICIAR PRUEBA
void prueba_iniciar() {

    pruebaFinalizada = false;

    // Comenzar el diezmado desde la primera muestra
    contadorDiezmado = 0;
}


// PROCESAR BLOQUE DEL MICROFONO
void prueba_procesar_bloque(int16_t *audioBlock, uint32_t numSamples) {

    if (audioBlock == nullptr) {
        return;
    }


    if (pruebaFinalizada) {
        return;
    }


    // Recorrer las muestras originales a 44.1 kHz
    for (uint32_t i = 0; i < numSamples; i++) {


        // Seleccionar una muestra cada 10
        if (contadorDiezmado == 0) {

            // Verificar espacio en el buffer
            if (muestrasMicrofono < TOTAL_MUESTRAS) {
                bufferMicrofono[muestrasMicrofono] = audioBlock[i];
                muestrasMicrofono++;
            }


            // Reiniciar contador del diezmado
            contadorDiezmado = DECIMATION_FACTOR - 1;
        }
        else {
            contadorDiezmado--;
        }


        // Verificar si terminó la adquisición
        if (muestrasMicrofono >= TOTAL_MUESTRAS) {
            pruebaFinalizada = true;
            break;
        }
    }
}


// VERIFICAR FINALIZACIÓN
bool prueba_finalizada() {
    return pruebaFinalizada;
}