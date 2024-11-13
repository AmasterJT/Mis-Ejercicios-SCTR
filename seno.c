#include <stdio.h>
#include <math.h>

#define N 400               // Número de puntos
#define DEG120 (2 * M_PI / 3) // 120 grados en radianes
#define DEG240 (4 * M_PI / 3) // 240 grados en radianes

void imprimeMatrizInt(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if ((i + 1) % 10 == 0) {  // Nueva fila cada 10 elementos
            printf("\n");
        } else {
            printf(", ");
        }
    }
    printf("\n");
}

void imprimeMatrizFloat(float arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%.2f", arr[i]);
        if ((i + 1) % 10 == 0) {  // Nueva fila cada 10 elementos
            // printf(",");
            printf("\n");
        } else {
            printf(", ");
        }
    }
    printf("\n");
}

void generaSenoides() {
    int sineInt[N];          // Seno en el rango de 0 a 80
    float sineNorm[N];       // Seno en el rango de -1 a 1
    float sineScaled[N];     // Seno en el rango de -5 a 5
    float sine10Scaled[N];   // Seno en el rango de -10 a 10
    float sineNorm120[N];    // Seno desplazado 120 grados, rango -1 a 1
    float sineNorm240[N];    // Seno desplazado 240 grados, rango -1 a 1
    int sineInt120[N];       // Seno desplazado 120 grados, rango 0 a 80
    int sineInt240[N];       // Seno desplazado 240 grados, rango 0 a 80

    // Generamos los valores del seno
    for (int i = 0; i < N; i++) {
        float angle = (2 * M_PI * i) / N;      // Ángulo en radianes para el seno
        sineNorm[i] = sin(angle);              // Seno entre -1 y 1
        sineInt[i] = (int)(40 + 40 * sineNorm[i]); // Escalado a rango 0 a 80
        sineScaled[i] = sineNorm[i] * 5;       // Escalado a rango -5 a 5
        sine10Scaled[i] = sineNorm[i] * 10;       // Escalado a rango -10 a 10

        // Seno con desfase de 120 y 240 grados
        sineNorm120[i] = sin(angle + DEG120);  // Desfasado 120 grados
        sineNorm240[i] = sin(angle + DEG240);  // Desfasado 240 grados

        // Escalado al rango 0 a 80 con desfase
        sineInt120[i] = (int)(40 + 40 * sineNorm120[i]);
        sineInt240[i] = (int)(40 + 40 * sineNorm240[i]);
    }

    // Imprimir los resultados
    // printf("Valores de seno en rango [0, 80] sin desfase:\n");
    // imprimeMatrizInt(sineInt, N);

    // printf("\nValores de seno en rango [-1, 1] sin desfase:\n");
    // imprimeMatrizFloat(sineNorm, N);

    // printf("\nValores de seno en rango [-5, 5] sin desfase:\n");
    // imprimeMatrizFloat(sineScaled, N);

    printf("\nValores de seno en rango [-10, 10] sin desfase:\n");
    imprimeMatrizFloat(sine10Scaled, N);

    // printf("\nValores de seno en rango [-1, 1] con desfase de 120 grados:\n");
    // imprimeMatrizFloat(sineNorm120, N);

    // printf("\nValores de seno en rango [-1, 1] con desfase de 240 grados:\n");
    // imprimeMatrizFloat(sineNorm240, N);

    // printf("\nValores de seno en rango [0, 80] con desfase de 120 grados:\n");
    // imprimeMatrizInt(sineInt120, N);

    // printf("\nValores de seno en rango [0, 80] con desfase de 240 grados:\n");
    // imprimeMatrizInt(sineInt240, N);
}

int main() {
    generaSenoides();
    return 0;
}