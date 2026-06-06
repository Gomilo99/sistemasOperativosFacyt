#include <stdio.h>

// VARIABLES COMPARTIDAS
int flag[2] = {0, 0};
int turn = 0;      // De quién es el turno en caso de conflicto
int saldo = 1000;

void proceso_0() {
    printf("P0: Levanto mi bandera (flag[0]=1)\n");
    flag[0] = 1;
    
    printf("P0: Esperando mi turno o que P1 no quiera...\n");
    while (flag[1] == 1 && turn == 1) {
        printf("  P0: P1 quiere Y es su turno. Esperando...\n");
    }
    
    // SECCIÓN CRÍTICA
    printf("P0: ¡ENTRÉ! Saldo: %d\n", saldo);
    saldo -= 500;
    printf("P0: Retiré 500. Saldo: %d\n", saldo);
    
    // Ceder turno a P1
    turn = 1;
    flag[0] = 0;
    printf("P0: Salí. Turn=%d\n\n", turn);
}

void proceso_1() {
    printf("P1: Levanto mi bandera (flag[1]=1)\n");
    flag[1] = 1;
    
    printf("P1: Esperando mi turno o que P0 no quiera...\n");
    while (flag[0] == 1 && turn == 0) {
        printf("  P1: P0 quiere Y es su turno. Esperando...\n");
    }
    
    // SECCIÓN CRÍTICA
    printf("P1: ¡ENTRÉ! Saldo: %d\n", saldo);
    saldo -= 300;
    printf("P1: Retiré 300. Saldo: %d\n", saldo);
    
    // Ceder turno a P0
    turn = 0;
    flag[1] = 0;
    printf("P1: Salí. Turn=%d\n\n", turn);
}

int main() {
    printf("===== ALGORITMO DE DEKKER - VERSIÓN 3 =====\n");
    printf("Saldo inicial: %d\n", saldo);
    printf("Turno inicial: %d (P0)\n\n", turn);
    
    // Simulación de interleaving
    printf("--- ESCENARIO: Ambos quieren entrar al mismo tiempo ---\n");
    flag[0] = 1;
    printf("P0 levanta bandera\n");
    flag[1] = 1;
    printf("P1 levanta bandera\n");
    
    // Ahora P0 entra primero (su turno)
    printf("\nP0 intenta entrar:\n");
    while (flag[1] == 1 && turn == 1) { }
    saldo -= 500;
    printf("P0: Saldo: %d\n", saldo);
    turn = 1;
    flag[0] = 0;
    
    printf("\nP1 intenta entrar:\n");
    while (flag[0] == 1 && turn == 0) { }
    saldo -= 300;
    printf("P1: Saldo: %d\n", saldo);
    turn = 0;
    flag[1] = 0;
    
    printf("\nSaldo final: %d (Debería ser: 200)\n", saldo);
    return 0;
}