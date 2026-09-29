#include <Arduino.h>
#include <Audio.h>

#include "buffers.h"
#include "generador_ruido.h"
#include "entrada_microfono.h"
#include "prueba_audio.h"
#include "wav.h"
#include "lms.h"
#include "guardar_coeficientes.h"


// Control del CoDec SGTL5000
AudioControlSGTL5000 sgtl5000;


// Generador de ruido blanco
GeneradorRuidoBlanco generadorRuido;


// Salida I2S
AudioOutputI2S audioOut;


// Frecuencia nativa del Audio Library / Audio Board
#define SAMPLE_RATE_NATIVE       44100


// Factor de diezmado
#define DECIMATION_FACTOR        10


// Generador
AudioConnection patchCordOutL(generadorRuido, 0, audioOut, 0);
AudioConnection patchCordOutR(generadorRuido, 0, audioOut, 1);


// Variables globales
bool pruebaActiva = false;
float s_buffer[LMS_FILTER_LEN];


// SETUP
void setup() {

    // MEMORIA DEL SISTEMA DE AUDIO
    AudioMemory(20);


    // INICIALIZAR SGTL5000
    sgtl5000.enable();
    sgtl5000.inputSelect(AUDIO_INPUT_LINEIN);
    sgtl5000.lineInLevel(5);
    sgtl5000.volume(0.5);


    // INICIALIZAR MICROSD
    if (!SD.begin(BUILTIN_SDCARD)) {
        while (true) {
            delay(1000);
        }
    }


    // INICIALIZAR MICROFONO
    entrada_microfono_init();


    // INICIALIZAR BUFFERS
    buffers_iniciar();


    // INICIALIZAR PRUEBA
    prueba_iniciar();


    // INICIALIZAR GENERADOR
    generadorRuido.iniciar();


    // ACTIVAR PRUEBA
    pruebaActiva = true;
}


// LOOP
void loop() {

    // CAPTURA DEL MICROFONO
    if (pruebaActiva) {
        if (entrada_microfono_disponible() > 0) {
            int16_t *audioBlock = entrada_microfono_leer();
            if (audioBlock != nullptr) {
                /*
                 * El Audio Library continúa entregando:
                 *
                 * 128 muestras
                 * a 44100 Hz
                 *
                 * La función prueba_procesar_bloque()
                 * debe realizar el diezmado 10:1 antes de
                 * guardar las muestras en los buffers.
                 */

                prueba_procesar_bloque(audioBlock, AUDIO_BLOCK_SAMPLES);

                entrada_microfono_liberar();
            }
        }
    }


    // FINALIZACIÓN
    if (prueba_finalizada()) {
        pruebaActiva = false;


        // GUARDAR LOS BUFFERS COMO WAV
        guardarWAV("ruido_emitido.wav", bufferRuido, TOTAL_MUESTRAS, SAMPLE_RATE);
        guardarWAV("microfono.wav", bufferMicrofono, TOTAL_MUESTRAS, SAMPLE_RATE);


        // CALCULAR COEFICIENTES DEL CAMINO SECUNDARIO
        calcular_camino_secundario(bufferRuido, bufferMicrofono, TOTAL_MUESTRAS, s_buffer);


        // GUARDAR COEFICIENTES
        guardar_coeficientes("coeficientes.txt", s_buffer, LMS_FILTER_LEN);


        while (1) {
            delay(1000);
        }
    }
}