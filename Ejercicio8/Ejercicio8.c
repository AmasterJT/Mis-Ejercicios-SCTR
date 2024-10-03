#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pid.h"


int main(int argc, char *argv[]) {

    incializaPID(0.001, 2, 0.3, 14); // inicializamos los valores T, Kp, Ki, Kd

    return 0;
}