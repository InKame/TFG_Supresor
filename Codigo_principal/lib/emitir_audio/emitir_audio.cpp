#include "emitir_audio.h"

/*
 * Constructor.
 *
 * Esta fuente de audio no tiene entradas físicas.
 * Tiene una salida de audio.
 */
EmitirAudio::EmitirAudio()
    : AudioStream(0, nullptr)
{
    indiceEntrada = 0;
    indiceSalida = 0;

    limpiarBuffer();
}


/*
 * Introduce una muestra float al buffer circular.
 *
 * La muestra debe estar aproximadamente entre:
 *
 *     -1.0  <= muestra <=  +1.0
 *
 * Si el buffer está lleno, la muestra se descarta.
 */
void EmitirAudio::enviarMuestra(float muestra)
{
    uint16_t siguiente;

    /*
     * Limitar la muestra al rango permitido.
     */
    if (muestra > 1.0f)
        muestra = 1.0f;

    if (muestra < -1.0f)
        muestra = -1.0f;

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
         * No bloqueamos el procesador.
         * Simplemente descartamos esta muestra.
         */
        return;
    }

    /*
     * Guardar muestra.
     */
    buffer[indiceEntrada] = muestra;

    /*
     * Actualizar índice de entrada.
     */
    indiceEntrada = siguiente;
}


/*
 * Vacía completamente el buffer.
 */
void EmitirAudio::limpiarBuffer()
{
    indiceEntrada = 0;
    indiceSalida = 0;

    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = 0.0f;
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
 *
 * Cada float del buffer se convierte a int16_t,
 * que es el formato utilizado internamente
 * por la Audio Library.
 */
void EmitirAudio::update(void)
{
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
     * Obtener 128 muestras.
     */
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
        uint16_t salida = indiceSalida;

        /*
         * Verificar si hay una muestra disponible.
         */
        if (salida != indiceEntrada)
        {
            float muestra = buffer[salida];

            /*
             * Convertir:
             *
             *     -1.0 ... +1.0
             *
             * a:
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

            block->data[i] = (int16_t)valor;


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
     * Enviar el bloque hacia la siguiente etapa
     * de la Teensy Audio Library.
     */
    transmit(block, 0);

    /*
     * Liberar el bloque.
     */
    release(block);
}
