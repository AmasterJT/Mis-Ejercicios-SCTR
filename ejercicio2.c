#include <math.h>
#include <stdio.h>

/*
EJERCICIO": {geometria analitica}
Codifica herramientas para el manejo de puntos y vectores en el espacio.Se
dispondrá de las siguientes operaciones:
    - Modulo de un vector
    - vector unitario en la direccion de un vctor v
    - suma de dos vectores
    - resta de dos vectores
    - multiuplicacion de un vector por un escalar
    - producto escalar de dos vectores
    - producto vectorial de dos vectores

Codifica también un programa para generar la secuencia de 20 posiciones donde
tiene que situarse sucesivamente el cabezal de un robot de soldadura para unir
dos planchas de hierro. En cada posición el cabezal realiza una soldadura
puntual, para lo que tiene que situarse a una distancia de 0.2 unidades de la
línea de unión de ambas planchas y a la misma distancia de ambas.

La primera plancha está anclada en los puntos p1, p2 y p3 cuyas coordenadas se
indican a continuación. La segunda plancha está anclada en p1, p2 y p4. La línea
de soldadura va de p1 a p2, la primera posición de soldadura es p1 y la última
p2.

    p1 = (5.07, 3.28, 2.16)
    p2 = (10.53, 12.19, 17.72)
    p3 = (21.01, 15.63, 6.97)
    p4 = (-3.15, 4.96, 0.32)
*/

typedef struct // estructura para definir un vector
{
  float x, y, z;
} Vector;

typedef struct // estructura para definir un punto
{
  float x, y, z;
} Punto;

Vector DefVectorPoints(Punto p1, Punto p2) {
  // para calcular el vector tenemos como origen p1 y extremo p2
  Vector v;
  v.x = p2.x - p1.x;
  v.y = p2.y - p1.y;
  v.z = p2.z - p1.z;

  return v;
}

// OPERACIONES CON VECTORES
// ---------------------------------------------------------

float module(Vector v) // funcion para calcular el modulo de un vector
{
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector Sum_vectors(Vector v1, Vector v2) {
  Vector vsum;
  vsum.x = v1.x + v2.x;
  vsum.y = v1.y + v2.y;
  vsum.z = v1.z + v2.z;
  return vsum;
}
Vector ProductByScalar(float Scalar, Vector v) {
  Vector result;
  result.x = v.x * Scalar;
  result.y = v.y * Scalar;
  result.z = v.z * Scalar;
  return result;
}

Vector cross_product(Vector v1, Vector v2) {
  Vector VCrossProduct;
  VCrossProduct.x = v1.y * v2.z - v2.y * v1.z;
  VCrossProduct.y = v1.z * v2.x - v2.z * v1.x;
  VCrossProduct.z = v1.x * v2.y - v2.x * v1.y;
  return VCrossProduct;
}

Vector unit_vector(Vector v) { return ProductByScalar(1 / module(v), v); }

Vector Res_vectors(Vector v1, Vector v2) {
  return Sum_vectors(v1, ProductByScalar(-1, v2)); // v1 - v2
}

float dot_product(Vector v1, Vector v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// ----------------------------------------------------------------------------------

// ECUACION DE LA RECTA EN FORMA VECTORIAL
// ==========================================

/*
    Conocemos dos puntos de la trayectoria recta p1 y p2, podemos calcular un
   vector director de esta recta

    Dado un punto P =(x,y,z) y un punto por el que pasa la recta A = (ax,ay,az)
   y tenemos el vector direccion V = (Vx,Vy,Vz)

    la recta será:
        x= ax + k+Vx
        y= ay + k+Vy
        z= az + k+Vz

        donde k es un parametro Real
*/

Punto PuntoRectaVectorial(float Parametro, Vector VectorDirector,
                          Punto PuntoOrigenRecta) {
  Punto PuntoRecta;

  PuntoRecta.x = PuntoOrigenRecta.x + Parametro * VectorDirector.x;
  PuntoRecta.y = PuntoOrigenRecta.y + Parametro * VectorDirector.y;
  PuntoRecta.z = PuntoOrigenRecta.z + Parametro * VectorDirector.z;
  return PuntoRecta;
}

// ==================================================================================

int main() {

  // parametros que definen las caracteristicas del proceso de soldadura
  float distanciaVertical = 0.2;
  int numPuntos = 20;

  // definimos los puntos de apoyo. para calcular la recta solo necitamos
  // conocer dso puntos pertenecientes a la recta
  Punto p1, p2; //, p3, p4;

  p1.x = 5.07;
  p1.y = 3.28;
  p1.z = 2.16;

  p2.x = 10.53;
  p2.y = 12.19;
  p2.z = 17.72;

  /*
  p3.x = 21.01;
  p3.y = 15.63;
  p3.z = 6.97;

  p4.x = -3.15;
  p4.y = 4.96;
  p4.z = 0.32;
  */

  Punto PuntosTrayectoria[numPuntos]; // matriz para guardar los puntos de la
                                      // trayectoria
  Vector VectorRecta =
      DefVectorPoints(p1, p2); // un vector director de la recta

  float paso = 1.0 / (numPuntos - 1);
  Vector Vpaso = ProductByScalar(
      paso,
      VectorRecta); // Vector que divide el segmennto p1p2 en 19 partes iguales

  Punto PuntoTrayectoria; // puntos de soldadura
  for (int k = 0; k < numPuntos; k++) {
    PuntoTrayectoria = PuntoRectaVectorial(k, Vpaso, p1);
    PuntoTrayectoria.z = PuntoTrayectoria.z +
                         distanciaVertical; // el punto que sigue el soldador
                                            // esta 0,2 por encia de la recta

    PuntosTrayectoria[k] = PuntoTrayectoria; // guadamos el punto de soldadura

    // imprimimos cada punto por consola
    printf("(%.2f", PuntoTrayectoria.x);
    printf(", %.2f", PuntoTrayectoria.y);
    printf(", %.2f", PuntoTrayectoria.z);
    printf(")\n");
  }

  return 0;
};