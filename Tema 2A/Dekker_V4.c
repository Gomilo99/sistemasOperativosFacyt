#include <stdio.h>

// VARIABLES COMPARTIDAS
int flag[2] = {0, 0};  // Intención de cada proceso
int turn = 0;          // De quién es el turno (0=P0, 1=P1)
int saldo = 1000;      // RECURSO CRÍTICO

void proceso_0() {
    flag[0] = 1;                   // "Quiero entrar"
    
    while (flag[1]) {              // Si P1 también quiere
        if (turn != 0) {           // Si NO es mi turno
            flag[0] = 0;           // Bajo mi bandera
            while (turn != 0);     // Espero mi turno
            flag[0] = 1;           // Levanto mi bandera de nuevo
        }
    }
    
    // SECCIÓN CRÍTICA
    saldo -= 500;
    
    turn = 1;          // Cedo turno a P1
    flag[0] = 0;       // "Ya no quiero"
}

void proceso_1() {
    flag[1] = 1;                   // "Quiero entrar"
    
    while (flag[0]) {              // Si P0 también quiere
        if (turn != 1) {           // Si NO es mi turno
            flag[1] = 0;           // Bajo mi bandera
            while (turn != 1);     // Espero mi turno
            flag[1] = 1;           // Levanto mi bandera de nuevo
        }
    }
    
    // SECCIÓN CRÍTICA
    saldo -= 300;
    
    turn = 0;          // Cedo turno a P0
    flag[1] = 0;       // "Ya no quiero"
}

int main() {
    printf("=== ALGORITMO DE DEKKER V4 ===\n");
    printf("Saldo inicial: %d\n\n", saldo);
    
    proceso_0();
    printf("Después de P0: saldo = %d\n", saldo);
    
    proceso_1();
    printf("Después de P1: saldo = %d\n", saldo);
    
    proceso_0();
    printf("Después de P0 (2da vez): saldo = %d\n\n", saldo);
    
    printf("Saldo final: %d (Esperado: 200)\n", saldo);
    
    printf("\n--- MEJORA EN V4 ---\n");
    printf("Si hay conflicto, el proceso baja su bandera y espera su turno.\n");
    printf("Cuando el turno llega, levanta su bandera de nuevo.\n");
    printf("Esto permite que P0 pueda entrar múltiples veces sin depender de P1.\n");
    
    return 0;
}

/*
CORRIDA EN FRÍO (DRY RUN):

=== ALGORITMO DE DEKKER V4 ===
Saldo inicial: 1000

Después de P0: saldo = 500
Después de P1: saldo = 200
Después de P0 (2da vez): saldo = -300

Saldo final: -300 (Esperado: 200)

--- MEJORA EN V4 ---
Si hay conflicto, el proceso baja su bandera y espera su turno.
Cuando el turno llega, levanta su bandera de nuevo.
Esto permite que P0 pueda entrar múltiples veces sin depender de P1.

NOVEDAD: Introduces el concepto de "intención de retirada"
Si detectas conflicto, cedes voluntariamente para permitir fairness.
*/
