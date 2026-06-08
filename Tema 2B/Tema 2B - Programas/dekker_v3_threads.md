#include <stdio.h>
#include "mutex.h"

// VARIABLES COMPARTIDAS
int estadoHilo[2] = {0, 0};
int turnoHilo = 0;
int saldo = 1000;

/*
Version 3: Bandera + turno.
La bandera indica que quiere entrar.
El turno resuelve el conflicto si ambos quieren entrar al mismo tiempo.
*/
void* proceso_0(void* arg) {
    /*
    Código que no afecta datos compartidos
    */

    estadoHilo[0] = 1;

    while (estadoHilo[1] && turnoHilo == 1) {
        /* Espera su turno */
    }

    // Entra a la seccion critica
    saldo -= 500;

    // Cambio de turno
    turnoHilo = 1;

    // Resto del codigo
    estadoHilo[0] = 0;
    return NULL;
}

/*
Hilo 1: misma lógica, pero con su propia bandera.
*/
void* proceso_1(void* arg) {
    /*
    Código que no afecta datos compartidos
    */

    estadoHilo[1] = 1;

    while (estadoHilo[0] && turnoHilo == 0) {
        /* Espera su turno */
    }

    // Entra a la seccion critica
    saldo -= 300;

    // Cambio de turno
    turnoHilo = 0;

    // Resto del codigo
    estadoHilo[1] = 0;
    return NULL;
}

int main() {
    printf("=== DEKKER V3 ===\n");
    printf("Saldo inicial: %d\n\n", saldo);

    ThreadPair procesos = crear_procesos(proceso_0, proceso_1);

    esperar_procesos(procesos);

    printf("\nSaldo final: %d\n", saldo);
    return 0;
}

/*
CORRIDA EN FRÍO:

Estado inicial:
saldo = 1000
estadoHilo[0] = 0
estadoHilo[1] = 0
turnoHilo = 0

Hilo 0:
- quiere entrar
- estadoHilo[0] = 1
- estadoHilo[1] = 0, entonces entra
- saldo = 1000 - 500 = 500
- turnoHilo = 1
- estadoHilo[0] = 0

Hilo 1:
- quiere entrar
- estadoHilo[1] = 1
- estadoHilo[0] = 0, entonces entra
- saldo = 500 - 300 = 200
- turnoHilo = 0
- estadoHilo[1] = 0

Saldo final:
200

Explicación:
- La bandera indica intención de entrar.
- El turno evita el deadlock cuando ambos quieren entrar.
- Si hay conflicto, solo entra el hilo que tiene el turno.
*/