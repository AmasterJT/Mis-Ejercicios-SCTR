#include <microcontrolador.h> // Declaración de funciones entradaAnalogica() y salidaAnalogica() y
#include <pid.h>              // Módulo que implanta un PID

// variable MS
int main() {
    unsigned int MSanterior = MS; // Guarda el instante actual
    PID pid;                      // Estructura para manejar un PID
    inicializaPID(1, 2, 0.3, 14, -10, 10, 0, 1, 0, &pid);
    // Inicializa el PID con período de 1 ms, Kp=2, Ki=0.3 y Kd=14
    // y una actuación entre -10 y 10. La referencia se lee en la entrada 0,
    // la salida de la planta en la entrada 1 y la actuación se aplica en la
    // salida 0
    autoajustePID(0, 1, 0, 5, 0.5, 1, &pid);
    // Autoajuste del PID
    while (1) {                 // Repite indefinidamente ...
        if (MS != MSanterior) { // Si está en otro milisengundo ...
            MSanterior = MS;    // Se prepara para el siguiente período
            actuacionPID(&pid); // Calcula la actuación del PID y la aplica
        }
    }
}