// Algoritmo de lectura y escritura del WAV //

#ifndef ARCHIVO_WAV_H
#define ARCHIVO_WAV_H

// Global Vars



// Functions
int iniciar_lectura_wav(const char *nombre_archivo);
float leer_muestra_wav(void);
void cerrar_wav(void);
int iniciar_escritura_wav(const char *nombre_archivo); 
void escribir_muestra_wav(float muestra); 
void cerrar_escritura_wav(void);

#endif
