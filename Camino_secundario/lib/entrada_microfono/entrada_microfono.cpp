#include "entrada_microfono.h"

#include <Audio.h>


// ============================================================
// OBJETOS AUDIO
// ============================================================

AudioInputI2S audioInput;

AudioRecordQueue recordQueue;


// ============================================================
// CONEXIÓN
// ============================================================

// Canal derecho de I2S -> RecordQueue

AudioConnection patchCordMic(
    audioInput,
    1,
    recordQueue,
    0
);


// ============================================================
// INICIALIZAR
// ============================================================

void entrada_microfono_init() {

    recordQueue.begin();
}


// ============================================================
// BLOQUES DISPONIBLES
// ============================================================

uint16_t entrada_microfono_disponible() {

    return recordQueue.available();
}


// ============================================================
// LEER BLOQUE
// ============================================================

int16_t* entrada_microfono_leer() {

    return recordQueue.readBuffer();
}


// ============================================================
// LIBERAR BLOQUE
// ============================================================

void entrada_microfono_liberar() {

    recordQueue.freeBuffer();
}