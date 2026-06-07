#include <stdio.h>

// VARIABLES COMPARTIDAS
int flag[2] = {0, 0};
int turn = 0;
int saldo = 1000;

void entrada(int i) {
    int j = 1 - i;
    flag[i] = 1;
    while (flag[j]) {
        if (turn != i) {
            flag[i] = 0;
            while (turn != i);
            flag[i] = 1;
        }
    }
}

void salida(int i) {
    turn = 1 - i;
    flag[i] = 0;
}

void proceso_0() {
    entrada(0);
    saldo -= 500;
    salida(0);
}

void proceso_1() {
    entrada(1);
    saldo -= 300;
    salida(1);
}

int main() {
    printf("=== DEKKER V5 (FINAL) ===\n");
    printf("Saldo inicial: %d\n\n", saldo);
    
    proceso_0();
    printf("P0: %d\n", saldo);
    
    proceso_1();
    printf("P1: %d\n", saldo);
    
    proceso_0();
    printf("P0: %d\n\n", saldo);
    
    printf("Final: %d (Esperado: 200)\n", saldo);
    return 0;
}