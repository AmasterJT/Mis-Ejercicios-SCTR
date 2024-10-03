/*
Un electrocardiógrafo recibe una señal proveniente de un sensor que genera una señal
con nivel de tensión muy bajo y que necesita de una etapa de amplificación analógica
para poder llevarla a un convertidor A/D. Este amplificador amplifica
también el ruido debido a interferencias electromagnéticas. La señal
amplificada tiene un rango máximo de  -3V a 3V. Para medir esta señal amplificada, en el microcontrolador de este aparato disponemos de un sistema de desarrollo con la función float mideSenal() que
devuelve un número real en ese rango

Hay que muestrear la señal con un período de muestreo T = 1 ms y hay que aplicarle un filtro Butterworth de 5º orden de tipo paso bajo y frecuencia de corte de 100 Hz para poder reducir el ruido. La
función de transferencia digital de este filtro es:

El filtro parte de condiciones iniciales nulas.
Disponemos también de un temporizador y dos funciones para su manejo:
    - void arrancaTemporizador(int s, int ns): arranca el temporizador, que genera temporizaciones sucesivas con un
    intervalo de s segundos y ns nanosegundos.
    - void esperaTemporizador(): la llamada a esta función no devuelve el control hasta que finalice una temporización.
El electrocardiógrafo dispone de una pantalla LCD de una resolución de 1024x768 puntos donde se muestra la señal filtrada correspondiente a los últimos 4 segundos. Hay que refrescar la gráfica a cada
2 segundos. Para el manejo de la pantalla disponemos de las siguientes funciones:
    - void borraPantalla(): borra las líneas dibujadas en la pantalla.
    - void dibujaLinea(int x1, int y1, int x2, int y2): dibuja
        una línea en la pantalla uniendo los puntos (x1, y1) y (x2, y2). El rango de coordenadas disponibles en la pantalla para dibujar líneas es de 0 a 1023 en el eje X y de 50 a 717 en el eje Y.

Codifica la aplicación que hay que instalar en este aparato.
*/

//* SOLUCION ----------------------------------------------------------------------------------------------------------------------------------------------

// TODO: ver imagen en ./'imagenes ejercicios5'/ejercicio5.png

//? tenemos que mostrar los 4 ultimos segundos en la pantalla -> como tenemos un periodo de muestreo de 1ms, tendremos 4seg/1ms=40000 muestras -> necesitamos un array de 40000 elementos que llamaremos
//? "y"

//? la ecuacion en diferencias para la salida nos indica que nocesitiamos conocer las 5 ultimas muestras que la señal, estas muestras las tenemos en el array "y"

//? para la entrada la ecuacion en diferencias nos indica que necesitamos conocer las 5 ultimas muestras, estas muestras las tenemos en el array "u" -> necesitamos que este array tenga un tamaño de 6,
//? porque necesitamos las ultimas 5 muestras + la muestra actual

// Declaración de las funciones
void arrancaTemporizador(int, int);
void esperaTemporizador();
float mideSenal();
void borraPantalla();
void dibujaLinea(int x1, int y1, int x2, int y2);

void filtro_Digital_Butterworth(float *u, float *y) {

    // actualizamos los valores de las señales, los desplazamos a la izquierda
    for (int i = 0; i <= 3998; i++)
        y[i] = y[i + 1]; // Desplaza la señal filtrada
    for (int i = 0; i <= 4; i++)
        u[i] = u[i + 1]; // Desplaza las medidas

    // aqui ya tenemos la ultima posicion de la matriz y para guardar el nuuevo valor filtrado
    // tambien tenemos la ultima posicion de la matriz u para guardar el nuevo valor de la señal

    u[5] = mideSenal(); // guardamos la ultima medida

    // aplicamos el filtro para obtener la señal filtrada actual
    y[3999] =
        2.9754 * y[3998] - 3.8060 * y[3997] + 2.5453 * y[3996] - 0.8811 * y[3995] + 0.1254 * y[3994] + 0.0013 * u[5] + 0.0064 * u[4] + 0.0128 * u[3] + 0.0128 * u[2] + 0.0064 * u[1] + 0.0013 * u[0];
};

void inicializa_disposiotivo(float *u, float *y) {

    // inicializa los arrays a cero y borra la pantalla

    // inicializamos los arrays
    for (int i = 0; i <= 3999; i++) {
        y[i] = 0;
    }
    for (int i = 0; i <= 5; i++) {
        u[i] = 0;
    }
    // inicializamos la pantalla
    borraPantalla();
}

void main() {
    float u[6];              // array para guardar las ultimas medidas de la señal de entrada y la entrada actual en la ultima posicion
    float y[4000];           // array para guardar las ultimas medidas de la señal de salida y la salida actual en la ultima posicion
    int numMuestras = 0;     // contador para saber cuantas muestras han pasado
    int periodosMedidos = 0; // contador para saber cuantos periodos de muestreo han pasado

    inicializa_disposiotivo(u, y);

    // factores de escala
    float escalaVertical = (717 - 50) / 6.0; // como la medida esta entre los valores -3 y 3 voltios el rango es 3 - (-3) = 6.0

    float escalaHorizontal = (1023.0 - 0) / 4000.0; // dividomos el ancho  de la pantalla entre el numero de muestras

    // cero de la grafica. punto medio en el eje Y
    float ceroGrafica = (768 - 0) / 2;

    // inicializamos el temporizador para 1ms
    arrancaTemporizador(0, 1000000);

    while (1) {
        // esperamos a que finalice la temporizacion
        esperaTemporizador();

        // actualizamos los valores de las señales y aplica el filtro
        filtro_Digital_Butterworth(u, y);

        // verificamos si ya obtimos los primeros 4 segundos para mostrarlos en la pantalla
        if (numMuestras < 4000) {

            // este if es necesario para poder mostrar la grafica cuando acabamos de encender el electrocardiógrafo

            numMuestras++;

            // una vez tenemos las primeras 4000 muestras siempre tendremos 4000 medidas disponibles
            // para mostrarlas en la pantalla, solo tendremos que actualizar los valores de las medidas en la matriz y
        }

        periodosMedidos++;

        // si llegamos a los 20000 periodos de muestreo, dibujamos la señal filtrada y ya tenemos las
        // primeras 40000 medidas
        if (periodosMedidos == 2000 && numMuestras == 4000) {

            periodosMedidos = 0; // reiniciamos la  cuenta de los dos segundos

            // para el primer punto
            int x1 = 0;
            int y1 = ceroGrafica + (y[3999] * escalaVertical);

            for (int i = 1; i < 4000; i++) {

                int x2 = i * escalaHorizontal;
                int y2 = ceroGrafica + (y[i] * escalaVertical);

                dibujaLinea(x1, y1, x2, y2);

                // actualizamos las coordenadas para el siguiente punto
                x1 = x2;
                y1 = y2;
            }
        }
    }
}