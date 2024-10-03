/*
 Un sistema embebido está conectado a una cámara de vídeo de 640x480 puntos de resolución. Cada punto de
una imagen se representa en niveles de gris mediante un byte sin signo (desde 0=negro hasta 255=blanco). Se
supone que la imagen ya se ha capturado y todos los bytes están disponibles en una matriz uint8_t m[307200]
creada en el archivo imagen.h suministrado.

Los bytes se organizan en esta matriz por filas de puntos en la imagen, desde la fila superior a la inferior y
dentro de cada fila, de izquierda a derecha. Codifica un algoritmo que calcule una serie de características de objetos
elipsoidales (de color oscuro, con valor menor que 100) existente en la escena (el fondo es de color claro, mayor
que 200). Para cada objeto hay que obtener su posición xc, yc, su dimensión máxima L. y su área (número de
puntos ocupados por el objeto).

El resultado debería ser:
        Objeto   xc       yc       L       área
        1        319.01   109.17   49.28   4759
        2        157.88   144.37   52.53   2710
        3        401.88   246.30   79.88   7210
        4        224.10   290.94   84.89   11887
*/

/// SOLUCION ----------------------------------------------------------------------------------------------------------------------------------------------
/// recorremos la matriz por filas de izquierda a derecha. Para identificar un objeto, si la fila de arriba es toda blanca y ahora encontramos un poxel oscuro, entonces es un objeto nuevo, por tanto,
/// mientras en la fila de arriba tengamos un pixel oscuro y este pixel este en contacto con algno de los pixeles de la fila actual seguimos en el mismo objeto. una vez encontramos ora fila en blanco,
/// lo marcamos como objeto terminado y lo guardamos en un vector de objetos.

#include <math.h>   // par sqrt
#include <stdint.h> // tipos de datos
#include <stdio.h>  // para imprimir en pantalla
#include <stdlib.h> // para inicializar variables

#include "./Ejercicio4_imagen/imagen.h" /// Camaño nos proporciono el fichero imagen.h

// int m[307200]; // esta matriz deberia estar inicializada en el archivo imagen.h

int genera_objetos() {

    /*
    *Docstring:

    * SUMARY:
    *    Damos formato a la matriz de imagen, para que cada objeto tenga un número único, y que el número de objeto sea el último objeto
    *    encontrado en la imagen.
    *
    *    recorremos la matriz por filas de izquierda a derecha. Para identificar un objeto, si la fila de arriba es toda blanca y
    *    ahora encontramos un poxel oscuro, entonces es un objeto nuevo, por tanto, mientras en la fila de arriba tengamos un pixel oscuro
    *    y este pixel este en contacto con algno de los pixeles de la fila actual seguimos en el mismo objeto. una vez encontramos ora fila en
    *    blanco, lo marcamos como objeto terminado.
    *
    * PARAMETERS:
    *    void
    *
    * RETURNS:
    *    int: número de objetos encontrados en la imagen
    */

    int nObjetos = 0; // nos indica el número de objetos encontrados en la imagen

    for (int y = 0; y < 480; y++) {     // Para cada coordenada Y
        for (int x = 0; x < 640; x++) { // Para cada coordenada X

            if (m[y * 640 + x] >= 100) // Si hay color claro
                m[y * 640 + x] = 255;  // se pone a 255

            else {          // Si hay color oscuro
                int x1 = x; // x1 columna del primero de uno o varios pixels oscuros consecutivos

                while (m[y * 640 + x1] < 100) { // Va hacia la derecha mientras haya pixels oscuro
                    x1++;
                }

                m[y * 640 + x1] = 255; // Marca el siguiente pixel claro a 255
                int x2 = x;            // Comienza en el primer pixel oscuro

                while (x2 < x1) { // Para todos los pixels oscuros consecutivos ...

                    if (m[(y - 1) * 640 + x2] != 255) { // Si encima hay un pixel de un objeto ...

                        for (int x3 = x; x3 < x1; x3++) { // Recorre todos los pixels oscuros consecutivos

                            m[y * 640 + x3] = m[(y - 1) * 640 + x2]; // y copia el número de objeto
                        }
                        break; // Termina el bucle while
                    }
                    x2++; // Si no hubo break, continúa con el siguiente pixel oscuro
                }
                if (x2 == x1) { // Si no salió por break, entonces esta es la primera fila de un objeto
                    nObjetos++; // Localizó un nuevo objeto en la imagen

                    for (int x3 = x; x3 < x1; x3++) { // Para la fila de pixels oscuros
                        m[y * 640 + x3] = nObjetos;   // los marca con el nuevo número de objeto
                    }
                }
                x = x1; // Para continuar después de la fila de pixels oscuros analizada
            }
        }
    }

    printf("nObejtos: %d\n", nObjetos);

    return nObjetos;
}

void analiza_objetos(int nObjetos) {

    /*
     * Docstring:
     *
     * SUMARY:
     *     Para cada objeto detectado en la imagen, calcula sus coordenadas, su área y su longitud máxima. Luego, muestra en pantalla los
     *     datos.
     *
     * PARAMETERS:
     *     int nObjetos: número de objetos detectados en la imagen
     *
     * RETURNS:
     *     void
     */

    for (int objeto = 1; objeto <= nObjetos; objeto++) { // Para cada objeto detectado en la imagen

        float xc = 0, yc = 0; // Para calcular las coordenadas del centro
        int nPuntos = 0;      // Para calcular el área del objeto

        for (int y = 0; y < 480; y++) // Para cada fila ...

            for (int x = 0; x < 640; x++) // Para cada columna ...

                if (m[y * 640 + x] == objeto) { // Si ese punto pertenece al objeto ...

                    nPuntos++; // Incrementa el área
                    xc += x;   // Acumula la coordenada X
                    yc += y;   // Acumula la coordenada Y
                }
        xc = (float)xc / nPuntos; // Media de las coordenadas X, con división entre float
        yc = (float)yc / nPuntos; // Obtiene la media de las coordenadas Y
        float L = 0;              // Para calcular la longitud máxima

        for (int y = 0; y < 480; y++) // Para cada fila

            for (int x = 0; x < 640; x++) { // Para cada columna

                if (m[y * 640 + x] == objeto) { // Si es un punto del objeto

                    float distancia = sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc)); // Distancia al centro

                    if (distancia > L) // Si es una distancia mayor
                        L = distancia; // la actualiza
                }
            }
        L = L * 2; // Es el doble de la mayor distancia al centro

        printf("Objeto %d en (%.2f, %.2f), L=%.2f, área=%d\n", objeto, xc, yc, L, nPuntos);
        // Visualiza en pantalla los datos, indicando con %d que se visualiza un entero en decimal y con
        // %.2f que se visualiza un número real con dos decimales
    }
}

int main() {

    int nObjetos = genera_objetos(); // obtenemos la cantidad de objetos y damos formato a la matriz

    analiza_objetos(nObjetos); // calculamos los datos de cada objeto

    return 0;
}