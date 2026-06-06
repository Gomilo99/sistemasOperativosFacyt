#include <stdio.h>
#include <stdint.h>

// VARIABLES COMPARTIDAS
int turn = 0;      // 0 = turno de P0, 1 = turno de P1
int saldo = 1000;  // RECURSO CRÍTICO

void proceso_0() {
    printf("P0: Intentando entrar a sección crítica\n");
    
    // ESPERA A SU TURNO
    while (turn != 0) {
        printf("P0: Esperando... (turn=%d)\n", turn);
    }
    
    // SECCIÓN CRÍTICA
    printf("P0: ¡ENTRÉ! Saldo actual: %d\n", saldo);
    int temp = saldo;
    temp -= 500;
    saldo = temp;
    printf("P0: Retiré 500. Saldo nuevo: %d\n", saldo);
    
    // LIBERA
    turn = 1;  // Ahora es turno de P1
    printf("P0: Salí. Turn=%d\n\n", turn);
}

void proceso_1() {
    printf("P1: Intentando entrar a sección crítica\n");
    
    // ESPERA A SU TURNO
    while (turn != 1) {
        printf("P1: Esperando... (turn=%d)\n", turn);
    }
    
    // SECCIÓN CRÍTICA
    printf("P1: ¡ENTRÉ! Saldo actual: %d\n", saldo);
    int temp = saldo;
    temp -= 300;
    saldo = temp;
    printf("P1: Retiré 300. Saldo nuevo: %d\n", saldo);
    
    // LIBERA
    turn = 0;  // Ahora es turno de P0
    printf("P1: Salí. Turn=%d\n\n", turn);
}

int main() {
    printf("===== ALGORITMO DE DEKKER - VERSIÓN 1 =====\n");
    printf("Saldo inicial: %d\n\n", saldo);
    
    // Simulación: P0, luego P1, luego P0 nuevamente
    proceso_0();
    proceso_1();
    proceso_0();
    
    printf("Saldo final: %d (Debería ser: 200)\n", saldo);
    return 0;
}