#include "pid.h"
#include <math.h>
#include <microcontrolador.h>
// "static" impide que las variables sean importadas en otros modulos

void inicializaPID(int T, double Kp, double Ki, double Kd, double uMin, double uMax, int nEntradaR, int nEntradaY, int nSalidaU, PID *p) {
    double Tms = T * 0.001; // Período en segundos
    p->T = T;
    p->uMin = uMin;
    p->uMax = uMax;
    p->nEntradaR = nEntradaR;
    p->nEntradaY = nEntradaY;
    p->nSalidaU = nSalidaU;
    // Guarda los parámetros
    p->q0 = Kp + Ki * Tms + Kd / Tms;
    p->q1 = -Kp - 2 * Kd / Tms;
    p->q2 = Kd / Tms;
    // Calcula los parámetros de la ecuación en diferencias
    p->u1 = 0;
    p->e1 = 0;
    p->e2 = 0;
    // Inicializa valores anteriores de actuación y error
}
void autoajustePID(int nEntradaR, int nEntradaY, int nSalidaU, double u, double yInicial, int T, PID *p) {
    double P[5] = {0, 0, 0, 0, 0}, A[5] = {0, 0, 0, 0, 0}; // Cinco últimos valores de período y amplitud
    double mediaP, mediaA;                                 // Media de período y de amplitud
    int finalizar = 0;                                     // Buleano para finalizar el autoajuste
    p->nEntradaR = nEntradaR;
    p->nEntradaY = nEntradaY;
    p->nSalidaU = nSalidaU;
    p->T = T;
    // Copia parámetros en la estructura
    salidaAnalogica(p->nSalidaU, u);
    while (entradaAnalogica(p->nEntradaY) < yInicial)
        ;
    // Aplica actuación inicial y espera a que la salida llegue a un valor
    while (!finalizar) {
        unsigned int MSinicial = MS;                     // Instante de inicio de un período de oscilación
        double yMax = 0, yMin = 0, y;                    // Para calcular la amplitud
        salidaAnalogica(p->nSalidaU, -u);                // Aplica actuación negativa
        while ((y = entradaAnalogica(p->nEntradaY)) > 0) // Mientras salida positiva ...
            if (y > yMax)                                // Si la salida tiene un valor mayor,
                yMax = y;                                // actualiza el máximo
        salidaAnalogica(p->nSalidaU, u);                 // Aplica actuación positiva
        while ((y = entradaAnalogica(p->nEntradaY)) < 0) // Mientras salida negativa ...
            if (y < yMin)                                // Si la salida tiene un valor menor,
                yMin = y;                                // actualiza el mínimo
        for (int i = 3; i >= 0; i--) {                   // Desplaza períodos y amplitudes
            P[i + 1] = P[i];
            A[i + 1] = A[i];
        }
        P[0] = (MS - MSinicial) * 0.001; // Pone nuevo valor de período
        A[0] = yMax - yMin;              // Pone nuevo valor de amplitud
        mediaP = 0;
        mediaA = 0;
        for (int i = 0; i < 5; i++) {
            mediaP += P[i]; // Suma los valores de período
            mediaA += A[i]; // Suma los valores de amplitud
        }
        mediaP /= 5; // Calcula la media
        mediaA /= 5;
        finalizar = 1;
        for (int i = 0; i < 5; i++) {
            if ((fabs(P[i] - mediaP) > 0.02 * mediaP) || (fabs(A[i] - mediaA) > 0.02 * mediaA))
                finalizar = 0;
        }
        // Si alguno de los valores está fuera del 2% de la media, entonces no hay que
        // finalizar
    }
    double Ku = 4 * u / (M_PI * mediaA);
    p->Kp = 0.6 * Ku;
    p->Ki = 1.2 * Ku / mediaP;
    p->Kd = 0.075 * Ku;
    // Ajusta los parámetros del PID
    double Tms = T * 0.001; // Período en segundos
    p->q0 = p->Kp + p->Ki * Tms + p->Kd / Tms;
    p->q1 = -p->Kp - 2 * p->Kd / Tms;
    p->q2 = p->Kd / Tms;
    // Calcula los parámetros de la ecuación en diferencias
    p->u1 = 0;
    p->e1 = 0;
    p->e2 = 0;
    // Inicializa valores anteriores de actuación y error
}
double actuacionPID(PID *p) {
    double r = entradaAnalogica(p->nEntradaR); // Señal de referencia
    double y = entradaAnalogica(p->nEntradaY); // Señal de salida de la planta
    double e = r - y;                          // Señal de error
    double u = p->u1 + p->q0 * e + p->q1 * p->e1 + p->q2 * p->e2;
    // Actuación generada mediante el PID
    if (u > p->uMax)
        u = p->uMax;
    if (u < p->uMin)
        u = p->uMin;
    // Limita la actuación entre un valor mínimo y máximo posibles
    salidaAnalogica(p->nSalidaU, u); // Aplica la actuación

    p->u1 = u;
    p->e2 = p->e1;
    p->e1 = e;
    // Guarda valores anteriores de actuación y error
    return u; // Devuelve la actuación aplicada
}