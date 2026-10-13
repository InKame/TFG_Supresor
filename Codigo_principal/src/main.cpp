#include <Arduino.h>
#include <Audio.h>

#include "entrada_microfono.h"
#include "fxlms.h"
#include "emitir_audio.h"



// Factor de reducción:
// 44100 / 5 = 8820 Hz
#define DECIMATION_FACTOR       5


// Frecuencia efectiva de procesamiento del Fx-LMS
#define FXLMS_SAMPLE_RATE       (AUDIO_SAMPLE_RATE / DECIMATION_FACTOR)


// Tamaño de bloque de la Audio Library
#define BUFFER_SIZE             AUDIO_BLOCK_SAMPLES


// Variables del algoritmo Fx-LMS
float d_n;
float e;
float y_n_prima;
float x_n;
float y_n_techo;
float x_n_techo;
float y_n;


// Buffers del algoritmo Fx-LMS
float x_buf[FILTER_LEN]       = {0.0f};
float x_prima_buf[FILTER_LEN] = {0.0f};
float y_n_buf[FILTER_LEN]     = {0.0f};
float w_z[FILTER_LEN] = {0.0f};


// Modelo estimado del camino secundario S^(z)
float s_techo[FILTER_LEN] = {
0.05321944f, 0.07362130f, 0.07049599f, 0.04731398f, 0.01224434f, -0.02366209f, -0.05077186f, -0.06306107f, 
-0.05849726f, -0.04042465f, -0.01477182f, 0.01101143f, 0.03054743f, 0.03949526f, 0.03634190f, 0.02256256f, 
0.00212154f, -0.01957403f, -0.03695925f, -0.04528461f, -0.04294681f, -0.03128511f, -0.01454171f, 0.00102419f, 
0.00896950f, 0.00546908f, -0.01053884f, -0.03579746f, -0.06381585f, -0.08703116f, -0.09831931f, -0.09361047f, 
-0.07331956f, -0.04128969f, -0.00466710f, 0.02827511f, 0.05078603f, 0.05876224f, 0.05219843f, 0.03474104f, 
0.01260988f, -0.00715729f, -0.01863554f, -0.01860059f, -0.00750327f, 0.01084701f, 0.03039857f, 0.04392249f, 
0.04576490f, 0.03262721f, 0.00493383f, -0.03392009f, -0.07741997f, -0.11769237f, -0.14729020f, -0.16035074f, 
-0.15405647f, -0.12934101f, -0.09047394f, -0.04492147f, -0.00044778f, 0.03564205f, 0.05932767f, 0.07065890f, 
0.07347354f, 0.07459577f, 0.08023872f, 0.09423030f, 0.11546344f, 0.13796553f, 0.15274736f, 0.15129539f, 
0.12908354f, 0.08821110f, 0.03691961f, -0.01242378f, -0.04797651f, -0.06227431f, -0.05468689f, -0.03128504f, 
-0.00183373f, 0.02472308f, 0.04273246f, 0.05186747f, 0.05564697f, 0.05789960f, 0.06092490f, 0.06354738f, 
0.06236757f, 0.05516738f, 0.04207722f, 0.02763666f, 0.01834464f, 0.01928176f, 0.03066418f, 0.04683042f, 
0.05735086f, 0.05236556f, 0.02745182f, -0.01391844f, -0.05980458f, -0.09539805f, -0.10833050f, -0.09438928f, 
-0.05949711f, -0.01673603f, 0.01872761f, 0.03565660f, 0.03107260f, 0.00992482f, -0.01864113f, -0.04512229f, 
-0.06469215f, -0.07656828f, -0.08270576f, -0.08448592f, -0.08063076f, -0.06764209f, -0.04298331f, -0.00858918f, 
0.02786778f, 0.05410919f, 0.05941445f, 0.03981116f, 0.00221561f, -0.03681502f, -0.05736324f, -0.04521609f
};


// Constante de cambio del Fx-LMS
#define MU 0.00000001f


// LÍMITE PARA LOS COEFICIENTES DE W(z)
#define W_RESET_LIMIT 50.0f


// ============================================================
// VARIABLES PARA LA REDUCCIÓN DE FRECUENCIA
// ============================================================

// Contador de muestras de 44.1 kHz
int decimationCounter = 0;

// Última salida calculada por el Fx-LMS.
// Se mantiene durante las muestras que no se procesa el algoritmo.
float y_n_hold = 0.0f;


// OBJETOS DE AUDIO
EmitirAudio emitirAudio;
AudioOutputI2S audioOut;
AudioControlSGTL5000 sgtl5000;


// CONEXIONES DE AUDIO
AudioConnection patchCordSalidaL(emitirAudio, 0, audioOut, 0);
AudioConnection patchCordSalidaR(emitirAudio, 0, audioOut, 1);


// SETUP
void setup() {

    Serial.begin(115200);
    delay(1000);


    // Memoria de audio
    AudioMemory(40);


    // Inicializar entrada de micrófono
    entrada_microfono_init();


    // Inicializar SGTL5000
    sgtl5000.enable();


    // Volumen de salida
    sgtl5000.volume(0.5);


    // Inicializar variables
    e          = 0.0f;
    x_n        = 0.0f;
    y_n       = 0.0f;
    y_n_prima = 0.0f;
    y_n_techo = 0.0f;
    x_n_techo = 0.0f;
    d_n        = 0.0f;


    // Limpiar buffers del Fx-LMS
    for (int i = 0; i < FILTER_LEN; i++) {
        x_buf[i]       = 0.0f;
        x_prima_buf[i] = 0.0f;
        y_n_buf[i]     = 0.0f;
        w_z[i]         = 0.0f;
    }


    // Limpiar variables de reducción de frecuencia
    decimationCounter = 0;
    y_n_hold = 0.0f;


    // Limpiar buffer de salida
    emitirAudio.limpiarBuffer();
}


// LOOP
void loop() {

    // Verificar si existe un bloque de audio
    if (entrada_microfono_disponible() > 0) {


        // Obtener bloque del micrófono
        int16_t *audioBlock = entrada_microfono_leer();


        if (audioBlock != nullptr) {


            // =================================================
            // PROCESAR LAS 128 MUESTRAS DEL BLOQUE
            //
            // La Audio Library sigue trabajando a 44.1 kHz.
            //
            // El Fx-LMS se actualiza solamente cada 5 muestras:
            //
            // 44100 / 5 = 8820 Hz
            // =================================================

            for (int i = 0; i < BUFFER_SIZE; i++) {


                // Leer muestra del micrófono a 44.1 kHz
                float muestra_44k = (float)audioBlock[i] / 32768.0f;


                // =================================================
                // SALIDA
                //
                // La Audio Library sigue necesitando una muestra
                // a 44.1 kHz.
                //
                // Mientras el Fx-LMS no se actualiza, se mantiene
                // la última salida calculada.
                // =================================================
                emitirAudio.enviarMuestra(y_n_hold);


                // =================================================
                // DECIMACIÓN
                //
                // Solo una de cada 5 muestras entra al Fx-LMS.
                //
                // 44.1 kHz / 5 = 8.82 kHz
                // =================================================
                decimationCounter++;


                if (decimationCounter >= DECIMATION_FACTOR) {
                    decimationCounter = 0;


                    // =============================================
                    // AQUÍ COMIENZA EL PROCESAMIENTO Fx-LMS
                    // =============================================


                    // Señal de error del micrófono
                    e = muestra_44k;


                    // Referencia para Feedback Fx-LMS
                    x_n = e - y_n_techo;


                    // Buffer de x(n)
                    buffer(x_n, x_buf);


                    // x'(n) = S^(z) * x(n)
                    x_n_techo = filtro_adaptativo(s_techo, x_buf);


                    // Buffer de x'(n)
                    buffer(x_n_techo, x_prima_buf);


                    // Adaptación de W(z)
                    coeficientes_w(w_z, e, x_prima_buf, MU);


                    // COMPROBAR COEFICIENTES DE W(z)
                    bool resetW = false;
                    for (int k = 0; k < FILTER_LEN; k++) {


                        // NaN o infinito
                        if (!isfinite(w_z[k])) {
                            resetW = true;
                            break;
                        }


                        // Coeficiente demasiado grande
                        if (fabsf(w_z[k]) > W_RESET_LIMIT) {
                            resetW = true;
                            break;
                        }
                    }


                    // REINICIAR W(z)
                    if (resetW) {
                        for (int k = 0; k < FILTER_LEN; k++) {
                            w_z[k] = 0.0f;
                        }
                    }


                    // y(n) = W(z) * x(n)
                    y_n = filtro_adaptativo(w_z, x_buf);


                    // Buffer de y(n)
                    buffer(y_n, y_n_buf);


                    // y'(n) = S^(z) * y(n)
                    y_n_techo = filtro_adaptativo(s_techo, y_n_buf);


                    // Guardar salida calculada
                    y_n_hold = y_n;
                }
            }


            // Liberar bloque del micrófono
            entrada_microfono_liberar();
        }
    }
}