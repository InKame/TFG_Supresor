#include "emitir_audio.h"


/*
 * Constructor.
 *
 * Esta fuente de audio no tiene entradas físicas.
 * Tiene una salida de audio.
 */
EmitirAudio::EmitirAudio()
    : AudioStream(0, nullptr) {
    indiceEntrada = 0;
    indiceSalida = 0;
    limpiarBuffer();
}


/*
 * Introduce una muestra float al buffer circular.
 *
 * La muestra debe estar aproximadamente entre:
 *
 *     -1.0 <= muestra <= +1.0
 *
 * La muestra se convierte inmediatamente a int16_t.
 */
void EmitirAudio::enviarMuestra(float muestra) {
    uint16_t siguiente;


    /*
     * Limitar la muestra al rango permitido.
     */
    if (muestra > 1.0f)
        muestra = 1.0f;

    if (muestra < -1.0f)
        muestra = -1.0f;


    /*
     * Convertir la muestra float a int16_t.
     *
     * La Audio Library utiliza:
     *
     *     -32768 ... +32767
     */
    float valor = muestra * 32767.0f;


    /*
     * Saturación.
     */
    if (valor > 32767.0f)
        valor = 32767.0f;

    if (valor < -32768.0f)
        valor = -32768.0f;


    int16_t muestraPCM = (int16_t)valor;


    /*
     * Calcular la siguiente posición.
     */
    siguiente = indiceEntrada + 1;

    if (siguiente >= BUFFER_SIZE)
        siguiente = 0;


    /*
     * Verificar si el buffer está lleno.
     */
    if (siguiente == indiceSalida)
    {
        /*
         * Buffer lleno.
         *
         * No bloquear el sistema.
         *
         * La muestra se descarta.
         */
        return;
    }


    /*
     * Guardar muestra.
     */
    buffer[indiceEntrada] = muestraPCM;


    /*
     * Actualizar índice de entrada.
     */
    indiceEntrada = siguiente;
}


/*
 * Vacía completamente el buffer.
 */
void EmitirAudio::limpiarBuffer() {
    indiceEntrada = 0;
    indiceSalida = 0;


    /*
     * No es estrictamente necesario borrar físicamente
     * todo el buffer porque los índices indican que está
     * vacío.
     *
     * Sin embargo, se limpia para dejar el estado definido.
     */
    for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }
}


/*
 * Esta función es llamada automáticamente por
 * la Teensy Audio Library.
 *
 * La Audio Library trabaja con bloques de:
 *
 *     AUDIO_BLOCK_SAMPLES = 128
 *
 * muestras.
 */
void EmitirAudio::update(void) {
    audio_block_t *block;


    /*
     * Solicitar un bloque de audio.
     */
    block = allocate();


    /*
     * Si no hay memoria disponible, salir.
     */
    if (!block)
        return;


    /*
     * Obtener un bloque completo de 128 muestras.
     */
    for (uint16_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        uint16_t salida = indiceSalida;


        /*
         * Verificar si hay una muestra disponible.
         */
        if (salida != indiceEntrada) {
            /*
             * Copiar muestra del buffer.
             */
            block->data[i] = buffer[salida];


            /*
             * Avanzar índice de salida.
             */
            salida++;

            if (salida >= BUFFER_SIZE)
                salida = 0;


            indiceSalida = salida;
        }
        else
        {
            /*
             * No hay muestras disponibles.
             *
             * Enviar silencio.
             */
            block->data[i] = 0;
        }
    }


    /*
     * Enviar el bloque hacia AudioOutputI2S.
     */
    transmit(block, 0);


    /*
     * Liberar el bloque.
     */
    release(block);
}
