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
 */
class EmitirAudio : public AudioStream {
public:

    EmitirAudio();

    /*
     * Introduce una muestra de audio.
     *
     * muestra:
     *     Valor float entre aproximadamente -1.0 y +1.0.
     */
    void enviarMuestra(float muestra);

    /*
     * Limpia el buffer interno.
     */
    void limpiarBuffer();

protected:

    /*
     * Función llamada automáticamente por la
     * Teensy Audio Library cada vez que necesita
     * un nuevo bloque de audio.
     */
    virtual void update(void);

private:

    /*
     * Buffer circular de muestras float.
     *
     * Debe ser suficientemente grande para absorber
     * pequeñas diferencias entre el productor y el
     * consumidor.
     */
    static const int BUFFER_SIZE = 1024;

    volatile float buffer[BUFFER_SIZE];

    volatile uint16_t indiceEntrada;
    volatile uint16_t indiceSalida;
};

#endif
