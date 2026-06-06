#include <stdio.h>

// VARIABLES COMPARTIDAS
int flag[2] = {0, 0};  // flag[0]=1 si P0 quiere entrar, flag[1]=1 si P1 quiere
int saldo = 1000;

void proceso_0() {
    printf("P0: Levanto mi bandera (flag[0]=1)\n");
    flag[0] = 1;  // "Quiero entrar"
    
    // ESPERA A QUE P1 NO ESTÉ DENTRO
    printf("P0: Revisando si P1 quiere entrar...\n");
    while (flag[1] == 1) {
        printf("P0: P1 también quiere. Esperando...\n");
    }
    
    // SECCIÓN CRÍTICA
    printf("P0: ¡ENTRÉ! Saldo: %d\n", saldo);
    saldo -= 500;
    printf("P0: Retiré 500. Saldo: %d\n", saldo);
    
    // LIBERA
    flag[0] = 0;  // "Ya no quiero"
    printf("P0: Bajo mi bandera. Salí.\n\n");
}

void proceso_1() {
    printf("P1: Levanto mi bandera (flag[1]=1)\n");
    flag[1] = 1;  // "Quiero entrar"
    
    // ESPERA A QUE P0 NO ESTÉ DENTRO
    printf("P1: Revisando si P0 quiere entrar...\n");
    while (flag[0] == 1) {
        printf("P1: P0 también quiere. Esperando...\n");
    }
    
    // SECCIÓN CRÍTICA
    printf("P1: ¡ENTRÉ! Saldo: %d\n", saldo);
    saldo -= 300;
    printf("P1: Retiré 300. Saldo: %d\n", saldo);
    
    // LIBERA
    flag[1] = 0;  // "Ya no quiero"
    printf("P1: Bajo mi bandera. Salí.\n\n");
}

int main() {
    printf("===== ALGORITMO DE DEKKER - VERSIÓN 2 =====\n");
    printf("Saldo inicial: %d\n\n", saldo);
    
    proceso_0();
    proceso_1();
    
    printf("Saldo final: %d (Debería ser: 200)\n", saldo);
    return 0;
}
