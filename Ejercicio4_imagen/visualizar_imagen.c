#include <stdio.h>
#include <stdint.h>
#include "imagen.h"  // Incluye la matriz de la imagen

#define WIDTH 640
#define HEIGHT 480

// Función para guardar la imagen en formato PGM
void guardarPGM(const char *filename, uint8_t *m) {
    FILE *file = fopen(filename, "wb");
    
    if (!file) {
        printf("Error al abrir el archivo para escribir.\n");
        return;
    }
    
    // Escribir la cabecera del archivo PGM
    fprintf(file, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    
    // Escribir los datos de la imagen (matriz de niveles de gris)
    fwrite(m, sizeof(uint8_t), WIDTH * HEIGHT, file);
    
    fclose(file);
    printf("Imagen guardada como %s\n", filename);
}

int main() {
    // Asume que 'm' está definida en imagen.h y tiene el tamaño correcto (640x480 bytes)
    guardarPGM("imagen.pgm", m);  // Guarda la imagen como un archivo PGM
    
    return 0;
}
