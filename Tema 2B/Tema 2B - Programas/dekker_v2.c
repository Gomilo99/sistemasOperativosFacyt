#include <stdio.h>

int flag[2] = {0, 0};
int saldo = 1000;

void proceso_0() {
    flag[0] = 1;
    while (flag[1]);
    saldo -= 500;
    flag[0] = 0;
}

void proceso_1() {
    flag[1] = 1;
    while (flag[0]);
    saldo -= 300;
    flag[1] = 0;
}

int main() {
    printf("=== DEKKER V2 ===\n");
    printf("Inicial: %d\n\n", saldo);
    
    proceso_0();
    printf("P0: %d\n", saldo);
    proceso_1();
    printf("P1: %d\n", saldo);
    
    printf("Final: %d\n", saldo);
    return 0;
}

/*
CORRIDA EN FRÍO:
=== DEKKER V2 ===
Inicial: 1000

P0: 500
P1: 200

Final: 200

PROBLEMA: ¿Qué pasa si ambos levantan flags simultáneamente?
flag[0]=1, flag[1]=1
→ P0 ve flag[1]=1, espera
→ P1 ve flag[0]=1, espera
→ DEADLOCK MUTUO
*/