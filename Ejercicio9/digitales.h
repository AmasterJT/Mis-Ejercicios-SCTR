#ifndef DIGITALES_HBBdfj
#define DIGITALES_H

#include <cstdint>

class ESDigitales {

    // Datos privados

  public:
    enum Tipo { ENTRADA, SALIDA };

    ESDigitales();  // Constructor, configura todas las señales como entradas
    ~ESDigitales(); // Destructor, configura todas las señales como entradas

    void configuraESDigitales(int primera, int cuantas, Tipo entradaOSalida);

    // Configura 'cuantas' señales, desde 'primera', como entradas o salidas según el buleano
    // 'entradaOSalida'

    int leeEntrada(int linea);
    // Devuelve el estado de la señal 'linea' en un buleano

    uint8_t lee8Entradas(int primera);
    // Devuelve en un byte el estado de las 8 señales que comienzan en la señal 'primera'

    uint16_t lee16Entradas(int primera);
    // Devuelve en 16 bits el estado de las 16 señales que comienzan en la señal 'primera'.

    void escribeSalida(int valor, int linea);
    // Modifica la salida digital en la señal 'linea' y la pone al valor 'valor'.

    void escribe8Salidas(uint8_t valor, int primera);
    // Modifica 8 salidas digitales comenzando en la señal 'primera' y las pone según los
    // bits de 'valor'

    void escribe16Salidas(uint16_t valor, int primera);
    // Modifica 16 salidas digitales comenzando en la señal 'primera' y las pone según los
    // bits de 'valor.
};

#endif // DIGITALES_H