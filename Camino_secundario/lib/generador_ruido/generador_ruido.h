#ifndef GENERADOR_RUIDO_H
#define GENERADOR_RUIDO_H

#include <Arduino.h>
#include <Audio.h>


// ============================================================
// CLASE GENERADOR DE RUIDO BLANCO
// ============================================================

class GeneradorRuidoBlanco : public AudioStream {

public:

    // Constructor
    GeneradorRuidoBlanco();

    // Inicializar generador
    void iniciar();

    // Obtener número de muestras almacenadas en el buffer
    uint32_t muestrasGeneradasActuales();

    // Función utilizada por Audio Library
    virtual void update(void) override;


private:

    // Generar una muestra de ruido
    int16_t generarRuido();


    // Semilla del generador pseudoaleatorio
    uint32_t semilla;


    // Número de muestras NATIVAS generadas
    uint32_t muestrasGeneradas;


    // Contador para el diezmado 10:1
    uint32_t contadorDiezmado;


    // Filtro del ruido
    float salidaFiltro;

    int16_t filtrarRuido(int16_t muestra);
};

#endif