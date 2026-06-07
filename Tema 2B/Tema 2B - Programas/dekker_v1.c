#include <stdio.h>

int turn = 0;
int saldo = 1000;

void proceso_0() {
    while (turn != 0);
    // Seccion Critica
    saldo -= 500;

    turn = 1;
    // Seccion Restante
}

void proceso_1() {
    while (turn != 1);
    saldo -= 300;
    turn = 0;
}

int main() {
    printf("=== DEKKER V1 ===\n");
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
=== DEKKER V1 ===
Inicial: 1000

P0: 500
P1: 200
P0: -300

Final: -300 (Esperado: 200)

PROBLEMA: Alternancia forzada. P0 no puede entrar 2 veces seguidas.
*/