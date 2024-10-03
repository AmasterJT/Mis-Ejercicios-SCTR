#ifndef PID_H
#define PID_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

/*
    ECUACION EN DIFERENCIAS PARA EL PID:
    u[k] = u[k-1] + q0*e[k] + q1*e[k-1] + q2*e[k-2]  

    donde:
    q0 = Kp * [1 + (T/Ti) + (Td/T)]
    q1 = -Kp * [1 + 2*(Td/T)]
    q2 = Kp*Td/T

*/


#endif // PID_H