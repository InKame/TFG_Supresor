#ifndef WAV_H
#define WAV_H

#include <Arduino.h>
#include <SD.h>

// Clase para escritura de archivos WAV
class WAVWriter
{
public:

    // Constructor
    WAVWriter();

    // Inicia un nuevo archivo WAV
    bool begin(
        const char *filename,
        uint32_t sampleRate,
        uint16_t channels,
        uint16_t bitsPerSample
    );

    // Escribe un bloque de muestras PCM
    bool writeSamples(
        const int16_t *samples,
        uint32_t numSamples
    );

    // Finaliza y corrige el encabezado WAV
    void end();

    // Indica si el archivo está abierto
    bool isOpen();

    // Número de bytes de audio escritos
    uint32_t dataBytes();

private:

    File file;

    uint32_t sampleRate;
    uint16_t channels;
    uint16_t bitsPerSample;

    uint32_t totalDataBytes;

    // Escritura de enteros en formato little-endian
    void writeLE16(uint16_t value);
    void writeLE32(uint32_t value);

    // Escritura del encabezado WAV
    void writeHeader(uint32_t dataSize);

    // Actualiza el tamaño del encabezado
    void updateHeader();
};

#endif