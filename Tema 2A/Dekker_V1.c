#include <stdio.h>

// VARIABLES COMPARTIDAS
int turn = 0;      // 0 = turno de P0, 1 = turno de P1
int saldo = 1000;  // RECURSO CRÍTICO

void proceso_0() {
    while (turn != 0);  // Espera su turno
    
    // SECCIÓN CRÍTICA
    saldo -= 500;
    
    turn = 1;  // Cede turno a P1
}

void proceso_1() {
    while (turn != 1);  // Espera su turno
    
    // SECCIÓN CRÍTICA
    saldo -= 300;
    
    turn = 0;  // Cede turno a P0
}

int main() {
    printf("=== ALGORITMO DE DEKKER V1 ===\n");
    printf("Saldo inicial: %d\n\n", saldo);
    
    // Simulación
    proceso_0();
    printf("Después de P0: saldo = %d\n", saldo);
    
    proceso_1();
    printf("Después de P1: saldo = %d\n", saldo);
    
    proceso_0();
    printf("Después de P0 (2da vez): saldo = %d\n\n", saldo);
    
    printf("Saldo final: %d (Esperado: 200)\n", saldo);
    return 0;
}

/*
CORRIDA EN FRÍO (DRY RUN):

=== ALGORITMO DE DEKKER V1 ===
Saldo inicial: 1000

Después de P0: saldo = 500
Después de P1: saldo = 200
Después de P0 (2da vez): saldo = -300

Saldo final: -300 (Esperado: 200)

PROBLEMA: Si P0 quiere entrar 2 veces seguidas, se queda esperando.
El algoritmo solo alterna: P0 → P1 → P0 → P1
*/
