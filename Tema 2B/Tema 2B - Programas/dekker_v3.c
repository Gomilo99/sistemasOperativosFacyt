#include <stdio.h>

int flag[2] = {0, 0};
int turn = 0;
int saldo = 1000;

void proceso_0() {
    flag[0] = 1;
    while (flag[1] && turn == 1);
    saldo -= 500;
    turn = 1;
    flag[0] = 0;
}

void proceso_1() {
    flag[1] = 1;
    while (flag[0] && turn == 0);
    saldo -= 300;
    turn = 0;
    flag[1] = 0;
}

int main() {
    printf("=== DEKKER V3 ===\n");
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
=== DEKKER V3 ===
Inicial: 1000

P0: 500
P1: 200
P0: -300

Final: -300

ANÁLISIS:
- flag[0]=1, P0 espera: flag[1]==1 && turn==1?
- Si turn=0 (su turno), NO espera → entra
- Después: turn=1, flag[0]=0

Ventaja: Ya no hay deadlock
Sigue siendo simple
*/