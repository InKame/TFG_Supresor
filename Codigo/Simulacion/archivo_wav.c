#include <stdio.h>
#include <stdint.h>
#include <string.h> 
#include <math.h>

#include "archivo_wav.h"

// Parámetros del audio 
#define SAMPLE_RATE 48000 
#define NUM_CHANNELS 1 
#define BITS_PER_SAMPLE 16 

// Archivo WAV 
static FILE *archivo_wav = NULL; 
static FILE *wav_file = NULL;

// Cantidad de muestras escritas 
static uint32_t numero_muestras = 0;



int iniciar_lectura_wav(const char *nombre_archivo) {
    wav_file = fopen(nombre_archivo, "rb");

    if (wav_file == NULL)
        return 0;

    // Saltar encabezado WAV de 44 bytes
    fseek(wav_file, 44, SEEK_SET);

    return 1;
}

float leer_muestra_wav(void) {
    int16_t muestra;

    if (wav_file == NULL)
        return 0.0f;

    if (fread(&muestra, sizeof(int16_t), 1, wav_file) != 1)
        return 0.0f;

    return (float)muestra / 32768.0f;
}

void cerrar_wav(void) {
    if (wav_file != NULL)
    {
        fclose(wav_file);
        wav_file = NULL;
    }
}

// --------------------------------------------------------- 
// Escribe un entero de 16 bits en formato little-endian 
// --------------------------------------------------------- 
static void escribir_uint16_le(uint16_t valor) { 
    uint8_t bytes[2]; bytes[0] = valor & 0xFF; 
    bytes[1] = (valor >> 8) & 0xFF; 
    fwrite(bytes, 1, 2, archivo_wav); 
} 

// --------------------------------------------------------- 
// Escribe un entero de 32 bits en formato little-endian 
// --------------------------------------------------------- 
static void escribir_uint32_le(uint32_t valor) { 
    uint8_t bytes[4]; 
    bytes[0] = valor & 0xFF; 
    bytes[1] = (valor >> 8) & 0xFF; 
    bytes[2] = (valor >> 16) & 0xFF; 
    bytes[3] = (valor >> 24) & 0xFF; 
    fwrite(bytes, 1, 4, archivo_wav); 
} 

// --------------------------------------------------------- 
// Iniciar escritura WAV 
// --------------------------------------------------------- 
int iniciar_escritura_wav(const char *nombre_archivo) { 
    uint32_t byte_rate; 
    uint16_t block_align; 
    archivo_wav = fopen(nombre_archivo, "wb"); 
    if (archivo_wav == NULL) { 
        return 0; 
    } 
    numero_muestras = 0; 
    byte_rate = SAMPLE_RATE * NUM_CHANNELS * BITS_PER_SAMPLE / 8; 
    block_align = NUM_CHANNELS * BITS_PER_SAMPLE / 8; 
    
    // ----------------------------------------------------- 
    // RIFF 
    // ----------------------------------------------------- 
    fwrite("RIFF", 1, 4, archivo_wav); 
    
    // Tamaño del archivo - 8 
    // Se actualiza al cerrar 
    escribir_uint32_le(0); 
    fwrite("WAVE", 1, 4, archivo_wav); 
    
    // ----------------------------------------------------- 
    // fmt 
    // ----------------------------------------------------- 
    fwrite("fmt ", 1, 4, archivo_wav); 
    
    // Tamaño del bloque fmt 
    escribir_uint32_le(16); 
    
    // Formato PCM 
    escribir_uint16_le(1); 
    
    // Número de canales 
    escribir_uint16_le(NUM_CHANNELS); 
    
    // Frecuencia de muestreo 
    escribir_uint32_le(SAMPLE_RATE); 
    
    // Byte rate 
    escribir_uint32_le(byte_rate); 
    
    // Block align 
    escribir_uint16_le(block_align); 
    
    // Bits por muestra 
    escribir_uint16_le(BITS_PER_SAMPLE); 
    
    // ----------------------------------------------------- 
    // data 
    // ----------------------------------------------------- 
    fwrite("data", 1, 4, archivo_wav); 
    
    // Tamaño de los datos 
    // Se actualiza al cerrar 
    escribir_uint32_le(0); 
    return 1; 
} 
    
// --------------------------------------------------------- 
// Escribir una muestra 
// --------------------------------------------------------- 
void escribir_muestra_wav(float muestra) { 
    int16_t muestra_pcm; 
    if (archivo_wav == NULL) { 
        return; 
    } 
        
    // Limitar la señal al rango [-1, 1] 
    if (muestra > 1.0f) { 
        muestra = 1.0f; 
    } else if (muestra < -1.0f) { 
        muestra = -1.0f; 
    } 
        
    // Convertir float [-1,1] a PCM de 16 bits 
    muestra_pcm = (int16_t)(muestra * 32767.0f); 
        
    // Escribir la muestra 
    escribir_uint16_le((uint16_t)muestra_pcm); numero_muestras++; 
} 
    
// --------------------------------------------------------- 
// Cerrar archivo WAV 
// --------------------------------------------------------- 
void cerrar_escritura_wav(void) { 
    uint32_t numero_bytes; 
    uint32_t tamano_archivo; 
    if (archivo_wav == NULL) { 
        return; 
    } 
        
    // Cada muestra ocupa 2 bytes 
    numero_bytes = numero_muestras * 2; 
        
    // Tamaño total del archivo 
    tamano_archivo = 44 + numero_bytes; 
        
    // ----------------------------------------------------- 
    // Actualizar tamaño del archivo RIFF 
    // ----------------------------------------------------- 
    fseek(archivo_wav, 4, SEEK_SET); 
    escribir_uint32_le(tamano_archivo - 8); 
        
    // ----------------------------------------------------- 
    // Actualizar tamaño del bloque data 
    // ----------------------------------------------------- 
    fseek(archivo_wav, 40, SEEK_SET); 
    escribir_uint32_le(numero_bytes); 
        
    // Cerrar archivo 
    fclose(archivo_wav); 
    archivo_wav = NULL; 
}
