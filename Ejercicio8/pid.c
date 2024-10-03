// "static" impide que las variables sean importadas en otros modulos

static double T, Kp, Ki, Kd; // no son variables globales porque no estan en el fichero ".h". Asi protegemos los datos

static double q0, q1, q2;

//CONDICIONES INCIALES NULAS
static double u1 = 0; // actuacion anterior 
static double e1 = 0, e2 = 0; // errores anteriores


void incializaPID(double PT, double PKp, double PKi, double PKd) {

    T = PT;
    Kp = PKp;
    Ki = PKi;
    Kd = PKd;


    q0 = Kp * [1 + (T/Ti) + (Td/T)];
    q1 = -Kp * [1 + 2*(Td/T)];
    q2 = Kp*Td/T;
}


double get_T() {
    return T;
}

double get_Kp() {
    return Kp;
}

double get_Ki() {
    return Ki;
}

double get_Kd() {
    return Kd;
}


double actuacionPID(double error){

    /*
        ECUACION EN DIFERENCIAS PARA EL PID:
        u[k] = u[k-1] + q0*e[k] + q1*e[k-1] + q2*e[k-2]  

        donde:
        q0 = Kp * [1 + (T/Ti) + (Td/T)]
        q1 = -Kp * [1 + 2*(Td/T)]
        q2 = Kp*Td/T

    */

    double u; // la actuacion actual del PID 
    u = u1 + q0*e1 + q1*e2 + q2*e1; // aplicamos la ecuacion en diferencias

    // actualizamos los valores para la proxima iteracion
    u1 = u;
    e2 = e1;
    e1 = error;

    return u;
}
