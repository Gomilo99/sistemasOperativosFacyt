#include <stdio.h>
#include "mutex.h"

// VARIABLES COMPARTIDAS
int estadoHilo[2] = {0, 0};
int saldo = 1000;

/*
Version 2: Solo banderas.
Cada hilo indica que quiere entrar a la sección crítica.
*/
void* hilo_0(void* arg) {
    /*
    Código que no afecta datos compartidos
    */
    while (estadoHilo[1]) {
        /* Espera su turno */
    }
    estadoHilo[0] = 1;
    // Entra a la seccion critica
    saldo -= 500;
    // Resto del codigo
    estadoHilo[0] = 0;
    return NULL;
}
/*
Hilo 1: misma lógica, pero con su propia bandera.
*/
void* hilo_1(void* arg) {
    /*
    Código que no afecta datos compartidos
    */
    while (estadoHilo[0]) {
        /* Espera su turno */
    }
    estadoHilo[1] = 1;
    // Entra a la seccion critica
    saldo -= 300;
    // Resto del codigo
    estadoHilo[1] = 0;
    return NULL;
}

int main() {
    printf("=== DEKKER V2 ===\n");
    printf("Saldo inicial: %d\n\n", saldo);

    ThreadPair procesos = crear_procesos(hilo_0, hilo_1);

    /*
    Espera a que ambos hilos terminen.
    */
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

Hilo 0:
- quiere entrar
- estadoHilo[0] = 1
- estadoHilo[1] = 0, entonces entra
- saldo = 1000 - 500 = 500
- estadoHilo[0] = 0

Hilo 1:
- quiere entrar
- estadoHilo[1] = 1
- estadoHilo[0] = 0, entonces entra
- saldo = 500 - 300 = 200
- estadoHilo[1] = 0

Saldo final:
200

Explicación:
- Cada hilo usa una bandera para indicar que quiere entrar.
- El problema es que si ambos levantan la bandera al mismo tiempo, pueden quedarse esperando indefinidamente.
- No hay variable de turno, por eso puede aparecer deadlock.
*/