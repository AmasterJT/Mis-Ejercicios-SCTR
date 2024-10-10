#ifndef PID_H
#define PID_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void incializaPID(double T, double Kp, double Ki, double Kd);

double actuacionPID(double error);

#endif // PID_H