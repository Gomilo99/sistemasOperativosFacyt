#include <stdio.h>
#include "mutex.h"

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

void* proceso_0(void* arg) {
    entrada(0);
    printf("P0: Entra. Saldo: %d\n", saldo);
    saldo -= 500;
    printf("P0: Retira 500. Saldo: %d\n", saldo);
    salida(0);
    return NULL;
}

void* proceso_1(void* arg) {
    entrada(1);
    printf("P1: Entra. Saldo: %d\n", saldo);
    saldo -= 300;
    printf("P1: Retira 300. Saldo: %d\n", saldo);
    salida(1);
    return NULL;
}

int main() {
    printf("=== DEKKER V5 (CON LIBRERÍA) ===\n");
    printf("Saldo inicial: %d\n\n", saldo);
    
    // Crear y ejecutar procesos (la librería maneja threads)
    ThreadPair procesos = crear_procesos(proceso_0, proceso_1);
    
    // Esperar a que terminen
    esperar_procesos(procesos);
    
    printf("\nSaldo final: %d (Esperado: 200)\n", saldo);
    return 0;
}