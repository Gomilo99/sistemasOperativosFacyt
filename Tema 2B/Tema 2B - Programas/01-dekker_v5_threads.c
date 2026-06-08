#include <stdio.h>
#include "mutex.h"

// VARIABLES COMPARTIDAS
int flag[2] = {0, 0};
int turn = 0;
int saldo = 1000;

// Protocolo de entrada: Indica que el hilo i quiere entrar a la sección crítica.
void entrada(int i) {
    int j = 1 - i;
    flag[i] = 1;

    while (flag[j]) {
        if (turn != i) {
            flag[i] = 0;
            while (turn != i) {
                /* Espera su turno */
            }
            flag[i] = 1;
        }
    }
}
// Protocolo de salida: Cede el turno al otro hilo y libera su intención.
void salida(int i) {
    turn = 1 - i;
    flag[i] = 0;
}

// Hilo 0: Retira 500 del saldo compartido.
void* hilo_0(void* arg) {
    // Código que no afecta datos compartidos
    entrada(0);

    // Entra a la sección crítica
    saldo -= 500;

    salida(0);
    // Resto del código
    return NULL;
}

// Hilo 1: Retira 300 del saldo compartido.
void* hilo_1(void* arg) {
    // Código que no afecta datos compartidos
    entrada(1);

    // Entra a la sección crítica
    saldo -= 300;

    salida(1);
    // Resto del código
    return NULL;
}

int main() {
    printf("=== DEKKER V5 ===\n");
    printf("Saldo inicial: %d\n\n", saldo);

    /*
    Se crea la ejecución concurrente.
    La librería mutex se encarga de iniciar ambos hilos.
    */
    ThreadPair procesos = crear_procesos(hilo_0, hilo_1);

    // Espera a que ambos hilos terminen.
    esperar_procesos(procesos);

    printf("\nSaldo final: %d\n", saldo);
    return 0;
}

/*
CORRIDA EN FRÍO:

Estado inicial:
saldo = 1000
flag[0] = 0
flag[1] = 0
turn = 0

Ejecución posible:

Hilo 0:
- quiere entrar
- flag[0] = 1
- flag[1] = 0, entonces entra
- saldo = 1000 - 500 = 500
- turn = 1
- flag[0] = 0

Hilo 1:
- quiere entrar
- flag[1] = 1
- flag[0] = 0, entonces entra
- saldo = 500 - 300 = 200
- turn = 0
- flag[1] = 0

Saldo final:
200

Explicación:
- la librería mutex permite crear y esperar hilos
- Dekker evita que ambos entren a la sección crítica al mismo tiempo
- el ejemplo del dinero se conserva igual
*/