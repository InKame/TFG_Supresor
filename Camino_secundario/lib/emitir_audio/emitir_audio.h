#ifndef EMITIR_AUDIO_H
#define EMITIR_AUDIO_H

#include <Arduino.h>
#include <Audio.h>


/*
 * Clase que permite introducir muestras float una por una
 * y entregarlas a la Teensy Audio Library.
 *
 * Las muestras se esperan normalizadas aproximadamente
 * entre -1.0 y +1.0.
 *
 * Internamente las muestras se almacenan como int16_t,
 * que es el formato utilizado por la Audio Library.
 */
class EmitirAudio : public AudioStream {
public:

    EmitirAudio();


    // Introduce una muestra de audio.
    void enviarMuestra(float muestra);


    // Limpia completamente el buffer interno.
    void limpiarBuffer();


protected:

    /*
     * Función llamada automáticamente por la
     * Teensy Audio Library.
     *
     * Cada llamada produce un bloque de:
     *
     *     AUDIO_BLOCK_SAMPLES = 128
     *
     * muestras.
     */
    virtual void update(void);


private:

    /*
     * Tamaño del buffer circular.
     *
     * 8192 muestras corresponden aproximadamente a:
     *
     *     8192 / 44100 = 185 ms
     *
     * Esto proporciona margen suficiente para pequeñas
     * pausas producidas por la lectura/escritura de la
     * microSD.
     */
    static const uint16_t BUFFER_SIZE = 16384;


    /*
     * Buffer circular de audio.
     *
     * Se almacenan directamente muestras int16_t.
     */
    volatile int16_t buffer[BUFFER_SIZE];


    /*
     * Índice donde se introduce la siguiente muestra.
     */
    volatile uint16_t indiceEntrada;


    /*
     * Índice de donde se obtiene la siguiente muestra.
     */
    volatile uint16_t indiceSalida;
};

#endif