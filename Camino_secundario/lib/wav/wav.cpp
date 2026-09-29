#include "wav.h"
#include <SD.h>


// ============================================================
// ESCRIBIR ENTERO DE 16 BITS
// ============================================================

static void escribirUInt16(File &archivo, uint16_t valor)
{
    archivo.write(valor & 0xFF);
    archivo.write((valor >> 8) & 0xFF);
}


// ============================================================
// ESCRIBIR ENTERO DE 32 BITS
// ============================================================

static void escribirUInt32(File &archivo, uint32_t valor)
{
    archivo.write(valor & 0xFF);
    archivo.write((valor >> 8) & 0xFF);
    archivo.write((valor >> 16) & 0xFF);
    archivo.write((valor >> 24) & 0xFF);
}


// ============================================================
// GUARDAR BUFFER COMO WAV
// ============================================================

bool guardarWAV(
    const char *nombreArchivo,
    const int16_t *buffer,
    uint32_t numeroMuestras,
    uint32_t frecuenciaMuestreo)
{
    // --------------------------------------------------------
    // Abrir archivo
    // --------------------------------------------------------

    File archivo = SD.open(nombreArchivo, FILE_WRITE);

    if (!archivo) {
        return false;
    }


    // --------------------------------------------------------
    // Parámetros WAV
    // --------------------------------------------------------

    const uint16_t canales = 1;

    const uint16_t bitsPorMuestra = 16;

    const uint32_t bytesPorMuestra =
        bitsPorMuestra / 8;

    const uint32_t bytesDatos =
        numeroMuestras * bytesPorMuestra;

    const uint32_t bytesPorSegundo =
        frecuenciaMuestreo *
        canales *
        bytesPorMuestra;


    // --------------------------------------------------------
    // RIFF
    // --------------------------------------------------------

    archivo.write("RIFF", 4);

    escribirUInt32(
        archivo,
        36 + bytesDatos
    );


    // --------------------------------------------------------
    // WAVE
    // --------------------------------------------------------

    archivo.write("WAVE", 4);


    // --------------------------------------------------------
    // fmt
    // --------------------------------------------------------

    archivo.write("fmt ", 4);

    escribirUInt32(archivo, 16);

    // PCM

    escribirUInt16(archivo, 1);

    // Número de canales

    escribirUInt16(
        archivo,
        canales
    );

    // Frecuencia de muestreo

    escribirUInt32(
        archivo,
        frecuenciaMuestreo
    );

    // Bytes por segundo

    escribirUInt32(
        archivo,
        bytesPorSegundo
    );

    // Block Align

    escribirUInt16(
        archivo,
        canales * bytesPorMuestra
    );

    // Bits por muestra

    escribirUInt16(
        archivo,
        bitsPorMuestra
    );


    // --------------------------------------------------------
    // data
    // --------------------------------------------------------

    archivo.write("data", 4);

    escribirUInt32(
        archivo,
        bytesDatos
    );


    // --------------------------------------------------------
    // Escribir muestras
    // --------------------------------------------------------

    archivo.write(
        (const uint8_t *)buffer,
        bytesDatos
    );


    // --------------------------------------------------------
    // Cerrar archivo
    // --------------------------------------------------------

    archivo.close();


    return true;
}