
/*
Codifica una subrutina para un controlador digital PID industrial gobernado por un microcontrolador con el
objetivo de realizar el ajuste automático (autotuning) de sus parámetros  Kp,Ki,Kd. El regulador dispone de dos
entradas analógicas. En la primera entrada se le suministra el valor de consigna  r(t). La segunda entrada se conecta la salida de la planta  y(t). A partir de ambas el regulador calcula la señal de
error  de seguimiento de la consigna. Mediante un algoritmo PID el controlador calcula la actuación sobre la planta  u(t) que se lleva a una salida analógica. Todas las señales tienen un rango de -10
V a 10 V.

El autoajuste de los parámetros Kp, Ki, Kd se realizará mediante el método del relé, utilizando el siguiente
procedimiento:
- Se supone que el sistema parte de un estado inicial en el que todas las señales están a 0 V antes de que se llame a
esta subrutina.
- Primero se aplicará una actuación de 5V hasta que la salida de la planta alcance 0.5 V.
- A continuación el controlador se convierte en un relé que genera una actuación de 5V si la salida de la planta es
negativa y que genera -5 V si la salida de la planta es positiva.
- El proceso de autoajuste finaliza cuando la oscilación en la salida de la planta se estabiliza presentando una
amplitud   y período   relativamente constantes en períodos de oscilación sucesivos. Se considera que cualquiera
de estos parámetros se mantiene relativamente constante cuando los 5 valores obtenidos de   y   en los últimos 5
períodos difieren de la media de esos 5 valores en menos del 2% de la media.  El valor final de   o
corresponde a la media de esos 5 últimos valores de   o  .
- Una vez determinados   y  , se aplican las siguientes fórmulas para calcular los parámetros del PID, donde  es D=5  la amplitud de la actuación aplicada en este proceso de autoajuste

Ku=(4*D)/(pi*A)
Kp=0.6*Ku
Ki=1.2*(Ku/P)
Kd=0.075*Ku
*/

//* SOLUCION -----------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#define D 5 // define la amplitud de la señal
#define PI 3.14159265359

void salidaAnalogica(float salida);
double entradaAnalogica(int entrada);
unsigned int MS; // represennta el periodo de oscilacion

float mediaDe5(float *numeros) {

    // Esta función recibe un array de 5 elementos y calcula su media. Es útil para determinar si los últimos valores de las amplitudes (A) y períodos (P) están estables.

    float med = 0;
    for (int i = 0; i < 5; i++)
        med += numeros[i];
    return med / 5;
}

void desplazaYAnade5(float *numeros, float add) {

    // Sirve para mantener un historial de los últimos 5 valores de amplitudes (A) y períodos (P).

    // Esta función desplaza un array de 5 elementos hacia la derecha, eliminando el último elemento y añadiendo el nuevo valor en la primera posición.

    for (int i = 5 - 1; i >= 0; i--) {
        if (i != 0)
            numeros[i] = numeros[i - 1];
        else
            numeros[i] = add;
    }
}

void ajustePID(float *pKp, float *pKi, float *pKd) {
    float P[5] = {0, 0, 0, 0, 0}, A[5] = {0, 0, 0, 0, 0};

    do {
        // Aquí se aplica una señal de 5V hasta que la salida de la planta alcanza 0.5V.
        salidaAnalogica(5);
    } while (entradaAnalogica(1) < 0.5);

    do {
        // Luego, se aplica una señal de -5V hasta que la salida de la planta cruza el cero.
        salidaAnalogica(-5);
    } while (entradaAnalogica(1) >= 0);

    // yPrev y yAct son las lecturas de la salida de la planta en dos instantes consecutivos. Se usan para detectar cruces de cero y alternar la señal de salida.
    float yPrev = 0;
    float yAct = entradaAnalogica(1);
    int condicion = 0;

    do {
        salidaAnalogica(5);
        MS = 0;
        float min = 0, max = 0;

        do {

            // Este bucle monitorea los valores máximos y mínimos de la salida de la planta durante
            // las oscilaciones inducidas por el controlador tipo rele

            if (yAct < min)
                min = yAct;
            if (yAct > max)
                max = yAct;

            yPrev = yAct;
            yAct = entradaAnalogica(1);

            if (yPrev < 0 && yAct >= 0)
                salidaAnalogica(-5);

        } while (!(yPrev > 0 && yAct <= 0));

        desplazaYAnade5(P, MS);
        desplazaYAnade5(A, max - min);
        float medP = mediaDe5(P);
        float medA = mediaDe5(A);

        for (int i = 0; i < 5; i++) {
            if (!(P[i] > medP * (1 - 0.02) && P[i] < medP * (1 + 0.02) && A[i] > medA * (1 - 0.02) && A[i] < medA * (1 + 0.02))) {
                condicion = 0;
                break;
            } else
                condicion = 1;
        }

    } while (!condicion);

    float Ku = 4 * D / PI / mediaDe5(A);
    *pKp = 0.6 * Ku;
    *pKi = 1.2 * Ku / mediaDe5(P);
    *pKd = 0.075 * Ku;
}

int main() {
    float Kp, Ki, Kd;
    ajustePID(&Kp, &Ki, &Kd);
    return 0;
}