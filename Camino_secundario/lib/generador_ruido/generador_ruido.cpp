#include "generador_ruido.h"
#include "buffers.h"


// CONSTRUCTOR
GeneradorRuidoBlanco::GeneradorRuidoBlanco()
    : AudioStream(0, nullptr)
{
    muestrasGeneradas = 0;

    contadorDiezmado = 0;

    semilla = 0x12345678;

    salidaFiltro = 0.0f;
}


// INICIALIZAR
void GeneradorRuidoBlanco::iniciar() {
    muestrasGeneradas = 0;
    contadorDiezmado = 0;
    semilla = 0x12345678;
    salidaFiltro = 0.0f;
}


// ============================================================
// OBTENER MUESTRAS GENERADAS
// ============================================================
//
// Devuelve el número de muestras almacenadas en bufferRuido.
//
// Es decir, muestras a 4410 Hz, NO muestras nativas a 44100 Hz.
// ============================================================
uint32_t GeneradorRuidoBlanco::muestrasGeneradasActuales() {
    return muestrasGeneradas;
}


// ============================================================
// GENERAR RUIDO BLANCO
// ============================================================

int16_t GeneradorRuidoBlanco::generarRuido() {
    // Xorshift32
    semilla ^= semilla << 13;
    semilla ^= semilla >> 17;
    semilla ^= semilla << 5;


    // Convertir a valor firmado
    int32_t ruido = (int32_t)(semilla & 0xFFFF) - 32768;


    // Reducir amplitud
    ruido /= 2;


    return (int16_t)ruido;
}


// ============================================================
// FILTRO PASO BAJO
// ============================================================
//
// El filtro se ejecuta a 44.1 kHz, antes del diezmado.
//
// Ruido blanco
//      ↓
// Filtro LP
//      ↓
// Diezmado 10:1
//      ↓
// bufferRuido a 4.41 kHz
//
// ============================================================

int16_t GeneradorRuidoBlanco::filtrarRuido(int16_t muestra) {
    const float alpha = 0.12f;
    salidaFiltro = salidaFiltro + alpha * ((float)muestra - salidaFiltro);
    return (int16_t)salidaFiltro;
}


// ============================================================
// UPDATE
// ============================================================

void GeneradorRuidoBlanco::update(void) {
    audio_block_t *block;


    // Solicitar bloque de Audio Library
    block = allocate();


    if (block == nullptr) {
        return;
    }


    // --------------------------------------------------------
    // Generar bloque de 128 muestras
    // --------------------------------------------------------
    //
    // Estas muestras continúan siendo de 44.1 kHz.
    //
    // El diezmado solamente afecta lo que se guarda
    // en bufferRuido.
    // --------------------------------------------------------

    for (uint16_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {


        // Generar ruido
        int16_t ruido = generarRuido();


        // Filtrar ruido
        int16_t muestra = filtrarRuido(ruido);


        // Enviar muestra al Audio Library
        block->data[i] = muestra;


        // ----------------------------------------------------
        // DIEZMADO 10:1
        // ----------------------------------------------------

        if (contadorDiezmado == 0) {


            // Guardar solamente una de cada 10 muestras
            if (muestrasGeneradas < TOTAL_MUESTRAS) {
                bufferRuido[muestrasGeneradas] = muestra;
                muestrasGeneradas++;
            }


            // Reiniciar contador
            contadorDiezmado = DECIMATION_FACTOR - 1;
        }
        else {
            contadorDiezmado--;
        }
    }


    // Enviar bloque a I2S
    transmit(block, 0);


    // Liberar bloque
    release(block);
}