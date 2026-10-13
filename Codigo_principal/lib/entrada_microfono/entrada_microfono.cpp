#include "entrada_microfono.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>


// Objetos de audio
AudioInputI2S audioInput;

// Cola para almacenar temporalmente los bloques recibidos
AudioRecordQueue recordQueue;

// Conexión:
// AudioInputI2S canal derecho -> AudioRecordQueue
AudioConnection patchCord1(audioInput, 1, recordQueue, 0);

// Control del codec SGTL5000
AudioControlSGTL5000 audioShield;


// Inicialización
void entrada_microfono_init() {
    // Memoria utilizada por la librería de audio
    AudioMemory(20);

    // Inicializar codec
    audioShield.enable();

    // Seleccionar entrada LINE IN
    audioShield.inputSelect(AUDIO_INPUT_LINEIN);

    // Nivel de entrada LINE IN
    // Puede ajustarse posteriormente según la señal del micrófono
    audioShield.lineInLevel(5);

    // Iniciar la cola de grabación
    recordQueue.begin();

    Serial.println("Entrada de audio inicializada.");
    Serial.println("Fuente: LINE IN R");
    Serial.println("Frecuencia de muestreo: 44.1 kHz");
    Serial.print("Muestras por bloque: ");
    Serial.println(AUDIO_BLOCK_SAMPLES);
    Serial.println();
}


// Cantidad de bloques disponibles
int entrada_microfono_disponible() {
    return recordQueue.available();
}


// Obtener bloque
int16_t* entrada_microfono_leer() {
    if (recordQueue.available() == 0) {
        return nullptr;
    }

    return recordQueue.readBuffer();
}


// Liberar bloque
void entrada_microfono_liberar() {
    recordQueue.freeBuffer();
}