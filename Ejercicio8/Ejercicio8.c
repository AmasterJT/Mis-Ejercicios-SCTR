#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pid.h"
#include "microcontrolador.h"


extern unsigned int MS; 

int main(int argc, char *argv[]) {

    incializaPID(0.001, 2, 0.3, 14); // inicializamos los valores T, Kp, Ki, Kd
    unsigned int MSanterior = MS; // obtenermos el valor incial del temporizador

    while(1) {

        if (MS != MSanterior) { // comprobamos si ha pasado el tiempo
            MSanterior = MS; // actualizamos el valor del temporizador

            double referencia = entradaAnalogica(0);
            double salida = entradaAnalogica(1);

            double error = referencia - salida;
            double u = actuacionPID(error);

            saldiaAnalogica(u); //aplicamos la salida a la planta
        }
    }
    return 0;   
}