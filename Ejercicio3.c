/*
En el manual de un módulo GPS se describe el protocolo de comunicación utilizado
para transferir infoGPS contínuamente con una cierta cadencia a través de un canal
de comunicación serie asíncrono a un sistema embebido que controla el
seguimiento de una trayectoria preprogramada en un dron


El módulo GPS envía paquetes de bytes con el siguiente contenido:
    - Bytes 0 y 1: dos bytes con los valores 0x04 y 0x24 como cabecera del
            paquete.
    - Bytes 2 a 5: cuatro bytes para expresar el instante de tiempo (dentro de
            un día de 24h) en el que se genera el paquete. Ejemplo: los bytes
            0x03, 0xDF, 0x15, 0x37 indican el valor 0x03DF1537 = 64951607.
            Dividiendo este valor por 1000 se obtiene el instante en formato
            hhmmss.sss. En el ejemplo, 64951607 / 1000 = 64951.607 = 6 hours,
            49 minutes y 51.607 seconds.
    - Bytes 6 a 9: cuatro bytes para expresar la latitud en grados decimales
            multiplicada por 1000000. Ejemplo: 0x01, 0x60, 0x74, 0xCC indican
            0x016074CC / 1000000 = 23.098572 grados.
    - Byte 10: con el valor 0x01 indica latitud N y con el valor 0x02 indica
            latitud S.
    - Bytes 11 a 14: cuatro bytes para expresar la longitud en grados decimales
            multiplicada por 1000000. Ejemplo: 0x07, 0x2B, 0x64, 0xDF indican
            0x072B64DF / 1000000 = 120.284383 grados.
    - Byte 15: con el valor 0x01 indica longitud E y con el valor 0x02 indica
            longitud W.
    - Byte 16: con el valor 0x01 indica que los infoGPS del paquete son válidos y
            con 0x02 que no lo son.
    - Bytes 17 a 20: cuatro bytes para expresar la dirección de movimiento con
            respecto al N. Ejemplo: 0x02, 0xB0, 0xE5, 0x7A indican 0x02B0E57A =
            45147514 indica que se está moviendo con un
            ángulo 45147514 / 1000000 = 45.1474514 grados.

    - Bytes 21 a 24: cuatro bytes para expresar la velocidad de movimiento en
            nudos/s. Ejemplo: 0x00, 0x00, 0xE8, 0x24 indica 0x0000E824 / 1000
            = 59.428 nudos/s.
    - Byte 25: el valor 0x4E indica que la velocidad se expresó en nudos/s.
    - Bytes 26 a 29: cuatro bytes para expresar la velocidad de movimiento en
            Km/h. Ejemplo: 0x00, 0x01, 0xAD, 0xED indican 0x0001ADED / 1000 =
            110.061 Km/h.
    - Byte 30: el valor 0x4B indica que la velocidad se expresó en Km/h.
    - Byte 31: es el checksum de todos los bytes anteriores salvo los de la
            cabecera, obtenido como el resultado de una O-exclusiva de todos
            ellos. En este ejemplo, 0x14.
    - Bytes 32 y 33: valores 0x0D y 0x0A que finalizan el paquete.

Una vez recibido por un canal de comunicaciones un paquete, el programa lo
almacena en una matriz de bytes como la que se indica a continuación. Codifica
una función a la que le pasamos este paquete de bytes y nos indica si el paquete
es válido y, en el caso de serlo, devuelve en una estructura la información
extraida.
*/

#include <stdint.h>
#include <stdio.h>

uint8_t mensaje[] = {0x04, 0x24,             // Cabecera del paquete
                     0x03, 0xDF, 0x15, 0x37, // Tiempo
                     0x01, 0x60, 0x74, 0xCC, // latitud
                     0x01,                   // Norte, Sur
                     0x07, 0x2B, 0x64, 0xDF, // longitud
                     0x01,                   // Este, Oeste
                     0x01,                   // Validez infoGPS
                     0x02, 0xB0, 0xE5, 0x7A, // grados direction norte
                     0x00, 0x00, 0xE8, 0x24, // Velocidadd
                     0x4E,                   // en nudos
                     0x00, 0x01, 0xAD, 0xED, // Velocidad
                     0x4B,                   // en km/h
                     0x14,                   // Checksum
                     0x0D, 0x0A};            // finalizacion del paquete

typedef struct { // estructura para extraer los infoGPS del mensaje enviado por el GPS
    int hour;
    int minutes;
    float seconds;
    float latitud, longitud, direction;
    float velocidadNudos, velocidadKmH;
} GPSdata;

GPSdata extract_data(uint8_t mensaje[]) {
    GPSdata infoGPS;

    // Extraer el tiempo
    uint32_t tiempo = (mensaje[2] << 24) | (mensaje[3] << 16) | (mensaje[4] << 8) | mensaje[5];
    infoGPS.hour = tiempo / 1000000;
    infoGPS.minutes = (tiempo % 1000000) / 10000;
    infoGPS.seconds = (tiempo % 10000) / 1000.0;

    // Extraer la latitud
    int32_t latitud = (mensaje[6] << 24) | (mensaje[7] << 16) | (mensaje[8] << 8) | mensaje[9];
    infoGPS.latitud = latitud / 1000000.0;
    if (mensaje[10] == 0x02) { // Si es sur
        infoGPS.latitud *= -1;
    }

    // Extraer la longitud
    int32_t longitud = (mensaje[11] << 24) | (mensaje[12] << 16) | (mensaje[13] << 8) | mensaje[14];
    infoGPS.longitud = longitud / 1000000.0;
    if (mensaje[15] == 0x02) { // Si es oeste
        infoGPS.longitud *= -1;
    }

    // Extraer la dirección de movimiento
    int32_t direction = (mensaje[17] << 24) | (mensaje[18] << 16) | (mensaje[19] << 8) | mensaje[20];
    infoGPS.direction = direction / 1000000.0;

    // Extraer la velocidad en nudos
    int32_t velocidadNudos = (mensaje[21] << 24) | (mensaje[22] << 16) | (mensaje[23] << 8) | mensaje[24];
    infoGPS.velocidadNudos = velocidadNudos / 1000.0;

    // Extraer la velocidad en km/h
    int32_t velocidadKmH = (mensaje[26] << 24) | (mensaje[27] << 16) | (mensaje[28] << 8) | mensaje[29];
    infoGPS.velocidadKmH = velocidadKmH / 1000.0;

    return infoGPS;
}

int checksum_comfirmation(uint8_t mensaje[]) {
    /*
        esta funcion devuleve un booleano cierto si el checksum recibido y el calculado coniciden
    */
    uint8_t checksum_mensagge = mensaje[31];
    // printf("%X\n", checksum_mensagge);

    // checksum_calculated is the checksum we calcute from the mensasge
    // if it the same as we recived then the message is correct
    // else the menssage is not correct
    uint8_t checksum_calculated = mensaje[2];

    for (int i = 3; i < 31; i++) {
        checksum_calculated = checksum_calculated ^ mensaje[i];
    };
    // printf("%X\n", checksum_calculated);

    if (checksum_calculated == checksum_mensagge) {
        return 1; // chechsum correct
    } else {
        return 0; // checksum incorrect
    }

    return 0;
}

int main() {
    // Verificar si el checksum es correcto
    if (checksum_comfirmation(mensaje)) {
        GPSdata infoGPS = extract_data(mensaje);

        // Mostrar los infoGPS extraídos
        printf("hour: %d:%d:%.3f\n", infoGPS.hour, infoGPS.minutes, infoGPS.seconds);
        printf("Latitud: %.6f\n", infoGPS.latitud);
        printf("Longitud: %.6f\n", infoGPS.longitud);
        printf("direction: %.6f grados\n", infoGPS.direction);
        printf("Velocidad: %.3f nudos, %.3f km/h\n", infoGPS.velocidadNudos, infoGPS.velocidadKmH);

    } else {
        printf("Paquete inválido (Checksum no coincide)\n");
    }
    return 0;
}