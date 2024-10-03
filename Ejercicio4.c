
// En una factoría hay 10 máquinas idénticas, desde la máquina 1 hasta la máquina 10. Para ejecutar la próxima
// orden de producción en una máquina determinada, se elige la máquina libre que menos órdenes ha ejecutado
// previamente. Codifica un algoritmo en lenguaje C que determine la máquina que tiene que ejecutar la siguiente
// orden, teniendo en cuenta la información indicada en las siguientes variables:

#include <stdio.h>
#include <stdlib.h>

int ordenes[] = {5, 3, 2, 6, 3, 4, 1, 6, 4, 8}; // Número de órdenes ejecutadas por cada máquina
int libres[] = {1, 1, 0, 0, 0, 1, 0, 0, 1, 1};  // Buleanos que indican qué máquinas están libres
int nMaquinas = 10;                             // Número de máquinas existentes

int maquinaActual;
int trabajosActuales;
int maquinaDisponible = 0;
int trabajosMaquinaDisponible = 0; // trabajo que esta realizando la maquina disponible

int main() {
    for (int i = 0; i < nMaquinas; i++) {

        maquinaActual = libres[i];     // vemos la disponibilidad
        trabajosActuales = ordenes[i]; // trabajos de la maquina actual (este ocupada o no)

        if (maquinaActual == 1) { // si la maquina esta libre

            if (maquinaDisponible == 0) // guardamos los datos de la primera maquina
            {
                maquinaDisponible = i + 1;
                trabajosMaquinaDisponible = trabajosActuales;
            }

            else if (trabajosActuales < trabajosMaquinaDisponible) {
                maquinaDisponible = i + 1;
                trabajosMaquinaDisponible = trabajosActuales;
            }
        }
    }

    if (maquinaDisponible == 0) {
        printf("todas las paquinas estan ocupadas\n");
        return 1; // codigo de estado de error
    }

    printf("Maquina disponible: %i\n trabajos realizados: %i\n", maquinaDisponible, trabajosMaquinaDisponible);

    return 0;
}
