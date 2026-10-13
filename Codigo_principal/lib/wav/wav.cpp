#include "wav.h"


// Constructor
WAVWriter::WAVWriter() {
    sampleRate = 44100;
    channels = 1;
    bitsPerSample = 16;

    totalDataBytes = 0;
}


// Inicialización
bool WAVWriter::begin(const char *filename, uint32_t fs,
    uint16_t numChannels, uint16_t bits) {
    sampleRate = fs;
    channels = numChannels;
    bitsPerSample = bits;

    totalDataBytes = 0;

    // Abrir archivo
    file = SD.open(filename, FILE_WRITE);

    if (!file) {
        return false;
    }

    // Escribir encabezado inicial
    // El tamaño del audio todavía es desconocido
    writeHeader(0);

    return true;
}


// Escritura del encabezado WAV
void WAVWriter::writeHeader(uint32_t dataSize) {
    uint32_t byteRate = sampleRate * channels * bitsPerSample / 8;
    uint16_t blockAlign = channels * bitsPerSample / 8;

    // RIFF
    file.write('R');
    file.write('I');
    file.write('F');
    file.write('F');

    // Tamaño del archivo - 8
    writeLE32(36 + dataSize);

    // WAVE
    file.write('W');
    file.write('A');
    file.write('V');
    file.write('E');

    // fmt
    file.write('f');
    file.write('m');
    file.write('t');
    file.write(' ');

    // Tamaño del bloque fmt
    writeLE32(16);

    // PCM
    writeLE16(1);

    // Número de canales
    writeLE16(channels);

    // Frecuencia de muestreo
    writeLE32(sampleRate);

    // Byte rate
    writeLE32(byteRate);

    // Block align
    writeLE16(blockAlign);

    // Bits por muestra
    writeLE16(bitsPerSample);

    // data
    file.write('d');
    file.write('a');
    file.write('t');
    file.write('a');

    // Tamaño de los datos
    writeLE32(dataSize);
}


// Escritura de 16 bits little-endian
void WAVWriter::writeLE16(uint16_t value) {
    file.write((uint8_t)(value & 0xFF));
    file.write((uint8_t)((value >> 8) & 0xFF));
}


// Escritura de 32 bits little-endian
void WAVWriter::writeLE32(uint32_t value) {
    file.write((uint8_t)(value & 0xFF));
    file.write((uint8_t)((value >> 8) & 0xFF));
    file.write((uint8_t)((value >> 16) & 0xFF));
    file.write((uint8_t)((value >> 24) & 0xFF));
}


// Escritura de muestras
bool WAVWriter::writeSamples(const int16_t *samples, uint32_t numSamples) {
    if (!file) {
        return false;
    }

    uint32_t bytes = numSamples * channels * bitsPerSample / 8;
    size_t written = file.write((const uint8_t *)samples, bytes);

    if (written != bytes) {
        return false;
    }

    totalDataBytes += bytes;

    return true;
}


// Actualización del encabezado
void WAVWriter::updateHeader() {
    // RIFF chunk size
    file.seek(4);
    writeLE32(36 + totalDataBytes);

    // DATA chunk size
    file.seek(40);
    writeLE32(totalDataBytes);
}


// Finalización
void WAVWriter::end() {
    if (!file) {
        return;
    }

    // Corregir tamaños del WAV
    updateHeader();

    // Cerrar archivo
    file.close();
}


// Estado del archivo
bool WAVWriter::isOpen() {
    return file;
}


// Bytes de audio
uint32_t WAVWriter::dataBytes() {
    return totalDataBytes;
}