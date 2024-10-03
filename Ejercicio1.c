/*
EJERCICIO 1:
En un proceso productivo en un momento dado se sabe que se están procesando como
máximo 10 piezas, cada una identificada mediante un código numérico. Se
proporciona en una matriz incidencias las incidencias que se han producido,
indicando para cada incidencia el número de pieza. También se indica en el
entero numIncidencias cuántas incidencias se han producido.  labora un algoritmo
que guarde en una matriz los códigos de las piezas y en otra matriz el  número
de incidencias por pieza.
*/

#include <stdio.h>

int main() {
    int incidencias[] = {85, 72, 91, 72, 72, 37, 37, 91, 37, 72, 37, 85, 37, 91};
    int numIncidencias = 14;

    // int longitud = sizeof(incidencias) / sizeof(incidencias[0]);
    // printf("%d", longitud);

    int ExisteMaquina = 0;
    int posicionador = 0;
    int ContadorIncidencias = 0;

    int maquinas[10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    int incidenciaMaquina[10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    // hallamos todas las maquinas que tienen alguna incidencia

    for (int i = 0; i < numIncidencias; i++) {
        int incidencia = incidencias[i];
        ExisteMaquina = 0;

        for (int j = 0; j < 10; j++) // vemos si la incidencia ya esta registrada en  el registro de maquinas
        {
            int maquina = maquinas[j];

            if (incidencia == maquina) {
                ExisteMaquina++; // la maquina ya esta registrada
            }
        }

        if (ExisteMaquina == 0) // la maquina no esta registrada
        {
            maquinas[posicionador] = incidencia; // registramos la maquina
            posicionador++;                      // cambiamos la posicion para guardar una maquina nueva
        }
    }

    // procedemos a contar la cantidad de incidencias que se produjeron en cada
    // maquina

    for (int i = 0; i < 10; i++) {
        ContadorIncidencias = 0;
        int maquina = maquinas[i];

        for (int j = 0; j < numIncidencias; j++) {
            if (maquina == incidencias[j]) {
                ContadorIncidencias++;
            }
        }

        if (maquina != 0) {
            incidenciaMaquina[i] = ContadorIncidencias;
        }
    }

    return 0;
}
