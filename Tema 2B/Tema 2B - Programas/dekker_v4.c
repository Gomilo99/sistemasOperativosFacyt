#include <stdio.h>

int flag[2] = {0, 0};
int turn = 0;
int saldo = 1000;

void proceso_0() {
    flag[0] = 1;
    while (flag[1]) {
        flag[0] = 0;
        while (turn != 0);
        flag[0] = 1;
    }
    saldo -= 500;
    turn = 1;
    flag[0] = 0;
}

void proceso_1() {
    flag[1] = 1;
    while (flag[0]) {
        flag[1] = 0;
        while (turn != 1);
        flag[1] = 1;
    }
    saldo -= 300;
    turn = 0;
    flag[1] = 0;
}

int main() {
    printf("=== DEKKER V4 ===\n");
    printf("Inicial: %d\n\n", saldo);
    
    proceso_0();
    printf("P0: %d\n", saldo);
    proceso_1();
    printf("P1: %d\n", saldo);
    proceso_0();
    printf("P0: %d\n\n", saldo);
    
    printf("Final: %d (Esperado: 200)\n", saldo);
    return 0;
}

/*
CORRIDA EN FRÍO:
=== DEKKER V4 ===
Inicial: 1000

P0: 500
P1: 200
P0: -300

Final: -300

NOVEDAD: Si hay conflicto, baja su bandera y espera su turno
Esto evita que se queden esperando indefinidamente
*/