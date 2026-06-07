#include <stdio.h>

// VARIABLES COMPARTIDAS
int flag[2] = {0, 0};  // flag[0] = intención de P0, flag[1] = intención de P1
int saldo = 1000;      // RECURSO CRÍTICO

void proceso_0() {
    flag[0] = 1;       // "Quiero entrar"
    while (flag[1]);   // Espera a que P1 no quiera
    
    // SECCIÓN CRÍTICA
    saldo -= 500;
    
    flag[0] = 0;       // "Ya no quiero"
}

void proceso_1() {
    flag[1] = 1;       // "Quiero entrar"
    while (flag[0]);   // Espera a que P0 no quiera
    
    // SECCIÓN CRÍTICA
    saldo -= 300;
    
    flag[1] = 0;       // "Ya no quiero"
}

int main() {
    printf("=== ALGORITMO DE DEKKER V2 ===\n");
    printf("Saldo inicial: %d\n\n", saldo);
    
    // Simulación sin conflicto
    proceso_0();
    printf("Después de P0: saldo = %d\n", saldo);
    
    proceso_1();
    printf("Después de P1: saldo = %d\n\n", saldo);
    
    printf("Saldo final: %d (Esperado: 200)\n", saldo);
    
    printf("\n--- ANÁLISIS DE DEADLOCK ---\n");
    printf("¿Qué pasaría si ambos levantaran flags SIMULTÁNEAMENTE?\n");
    printf("1. flag[0] = 1\n");
    printf("2. flag[1] = 1\n");
    printf("3. P0: ¿flag[1]==0? NO → espera\n");
    printf("4. P1: ¿flag[0]==0? NO → espera\n");
    printf("5. ¡DEADLOCK MUTUO!\n");
    
    return 0;
}

/*
CORRIDA EN FRÍO (DRY RUN):

=== ALGORITMO DE DEKKER V2 ===
Saldo inicial: 1000

Después de P0: saldo = 500
Después de P1: saldo = 200

Saldo final: 200 (Esperado: 200)

--- ANÁLISIS DE DEADLOCK ---
¿Qué pasaría si ambos levantaran flags SIMULTÁNEAMENTE?
1. flag[0] = 1
2. flag[1] = 1
3. P0: ¿flag[1]==0? NO → espera
4. P1: ¿flag[0]==0? NO → espera
5. ¡DEADLOCK MUTUO!

PROBLEMA: Sin variable turn, cuando hay conflicto ambos se bloquean.
*/
