#ifndef MICROCONTROLADOR_H
#define MICROCONTROLADOR_H


extern unsigned int MS; // Contador de milisegundos transcurridos
double entradaAnalogica(int nEntrada); // Devuelve la lectura de una entrada analógica
void salidaAnalogica(int nSalida, double valor); // Modifica una salida analógica



#endif // MICROCONTROLADOR_H