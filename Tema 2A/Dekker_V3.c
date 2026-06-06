#include <stdio.h>

// VARIABLES COMPARTIDAS
int flag[2] = {0, 0};  // Intención de cada proceso
int turn = 0;          // De quién es el turno (0=P0, 1=P1)
int saldo = 1000;      // RECURSO CRÍTICO

void proceso_0() {
    flag[0] = 1;                    // "Quiero entrar"
    while (flag[1] && turn == 1);   // Espera si P1 quiere Y es su turno
    
    // SECCIÓN CRÍTICA
    saldo -= 500;
    
    turn = 1;          // Cede turno a P1
    flag[0] = 0;       // "Ya no quiero"
}

void proceso_1() {
    flag[1] = 1;                    // "Quiero entrar"
    while (flag[0] && turn == 0);   // Espera si P0 quiere Y es su turno
    
    // SECCIÓN CRÍTICA
    saldo -= 300;
    
    turn = 0;          // Cede turno a P0
    flag[1] = 0;       // "Ya no quiero"
}

int main() {
    printf("=== ALGORITMO DE DEKKER V3 ===\n");
    printf("Saldo inicial: %d\n\n", saldo);
    
    proceso_0();
    printf("Después de P0: saldo = %d\n", saldo);
    
    proceso_1();
    printf("Después de P1: saldo = %d\n", saldo);
    
    proceso_0();
    printf("Después de P0 (2da vez): saldo = %d\n\n", saldo);
    
    printf("Saldo final: %d (Esperado: 200)\n", saldo);
    
    printf("\n--- ANÁLISIS ---\n");
    printf("Combinación de flag + turn evita deadlock.\n");
    printf("Flag indica intención, turn rompe empates.\n");
    printf("Condición: (flag[j] && turn==j) → ambas deben ser verdad\n");
    printf("Si NO es tu turno, esperas. Si SÍ es tu turno, entras.\n");
    
    return 0;
}

/*
CORRIDA EN FRÍO (DRY RUN):

=== ALGORITMO DE DEKKER V3 ===
Saldo inicial: 1000

Después de P0: saldo = 500
Después de P1: saldo = 200
Después de P0 (2da vez): saldo = -300

Saldo final: -300 (Esperado: 200)

--- ANÁLISIS ---
Combinación de flag + turn evita deadlock.
Flag indica intención, turn rompe empates.
Condición: (flag[j] && turn==j) → ambas deben ser verdad
Si NO es tu turno, esperas. Si SÍ es tu turno, entras.

VENTAJA: No hay deadlock. Pero sigue alternando.
*/
