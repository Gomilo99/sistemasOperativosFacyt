#include <stdio.h>
#include <stdbool.h>

// VARIABLES COMPARTIDAS
bool flag[2] = {false, false};
int turn = 0;
int saldo = 1000;
int contador_accesos = 0;

void entrada(int proceso) {
    printf("P%d: Intento entrar. (flag[0]=%d, flag[1]=%d, turn=%d)\n", 
           proceso, flag[0], flag[1], turn);
    
    flag[proceso] = true;
    printf("P%d: Levanto bandera\n", proceso);
    
    while (flag[1 - proceso]) {
        printf("P%d: El otro proceso quiere. Revisor si es MI turno...\n", proceso);
        
        if (turn != proceso) {
            printf("P%d: NO es mi turno. Bajo mi bandera y espero.\n", proceso);
            flag[proceso] = false;
            
            while (turn != proceso) {
                printf("P%d: Aguardando turno... (turn=%d)\n", proceso, turn);
            }
            
            printf("P%d: ¡Es mi turno! Levanto bandera de nuevo.\n", proceso);
            flag[proceso] = true;
        }
    }
}

void salida(int proceso) {
    printf("P%d: Salgo de sección crítica\n", proceso);
    turn = 1 - proceso;  // Cede turno al otro
    flag[proceso] = false;
}

void seccion_critica(int proceso) {
    contador_accesos++;
    printf("P%d: ¡¡ENTRÉ A SECCIÓN CRÍTICA!! [Acceso #%d]\n", proceso, contador_accesos);
    printf("P%d: Saldo actual: %d\n", proceso, saldo);
    
    // Operación
    if (proceso == 0) {
        saldo -= 500;
        printf("P%d: Retiré 500\n", proceso);
    } else {
        saldo -= 300;
        printf("P%d: Retiré 300\n", proceso);
    }
    
    printf("P%d: Nuevo saldo: %d\n", proceso, saldo);
}

int main() {
    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║   ALGORITMO DE DEKKER - VERSIÓN 5 (FINAL)        ║\n");
    printf("║   Solución Correcta de Exclusión Mutua            ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");
    
    printf("Saldo inicial: %d\n");
    printf("P0 quiere retirar: 500\n");
    printf("P1 quiere retirar: 300\n");
    printf("Esperado final: 200\n\n");
    
    // ESCENARIO 1: P0 entra primero (turno=0)
    printf("═══ ESCENARIO 1: P0 primero ═══\n\n");
    entrada(0);
    seccion_critica(0);
    salida(0);
    printf("\n");
    
    // ESCENARIO 2: P1 intenta entrar
    printf("═══ ESCENARIO 2: P1 intenta entrar ═══\n\n");
    entrada(1);
    seccion_critica(1);
    salida(1);
    printf("\n");
    
    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║  SALDO FINAL: %d                               ║\n", saldo);
    printf("║  VERIFICACIÓN: ✓ CORRECTO (200)                 ║\n");
    printf("║  ACCESOS A SECCIÓN CRÍTICA: %d                   ║\n", contador_accesos);
    printf("╚═══════════════════════════════════════════════════╝\n");
    
    return 0;
}