#ifndef PID_H
#define PID_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double Kp, Ki, Kd;        // Parámetros del PID
    int T;                    // Período en milisegundos
    double uMin, uMax;        // Mínimo y máximo posible en actuación
    double u1, e1, e2;        // Valores anteriores para implantar el PID
    double q0, q1, q2;        // Constantes para implantar el PID
    int nEntradaR, nEntradaY; // Entradas analógicas para referencia y medida de la salida
    int nSalidaU;             // Salida analógica para la actuación
} PID;                        // Estructura para implantar un PID

void inicializaPID(int T,                           // Período de muestreo en milisegundos
                   double Kp, double Ki, double Kd, // Parámetros del PID
                   double uMin, double uMax,        // Mínimo y máximo del valor de la actuación
                   int nEntradaR, int nEntradaY,    // Entradas analógicas para referencia y medida de la salida
                   int nSalidaU,                    // Salida analógica para la actuación
                   PID *p);                         // Puntero a la estructura a inicializar
// Inicializa un PID representado por una estructura de tipo PID

void autoajustePID(int nEntradaR, int nEntradaY, // Entradas analógicas para referencia y salida
                   int nSalidaU,                 // Salida analógica para actuación
                   double u,                     // Valor de actuación a aplicar en el procedimiento de autoajuste
                   double yInicial,              // Valor mínimo inicial de salida en el procedimiento de autoajuste
                   int T,                        // Período de muestreo en ms
                   PID *p);                      // Puntero a la estructura que representa el PID
// Inicializa un PID con un procedimiento de autoajuste para determinar sus parámetros

double actuacionPID(PID *p);
// Calcula la actuación del PID, la aplica a la salida analógica y la devuelve

#endif // PID_H