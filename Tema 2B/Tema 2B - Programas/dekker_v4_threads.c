#include <stdio.h>
#include "mutex.h"

// VARIABLES COMPARTIDAS
int estadoHilo[2] = {0, 0};
int turnoHilo = 0;
int saldo = 1000;

/*
Version 4: Bandera + turno con retirada temporal.
Si el hilo no tiene el turno, baja su bandera, espera, y luego la levanta otra vez.
*/
void* proceso_0(void* arg) {
    /*
    Código que no afecta datos compartidos
    */

    estadoHilo[0] = 1;

    while (estadoHilo[1]) {
        estadoHilo[0] = 0;
        // Cede turno al siguiente hilo
        while (turnoHilo != 0) {
            /* Espera su turno */
        }
        // Un tiempo de espera para que pase el siguiente
        estadoHilo[0] = 1;
    }

    // Entra a la seccion critica
    saldo -= 500;

    // Cambio de turno
    turnoHilo = 1;
    estadoHilo[0] = 0;

    // Resto del codigo
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

    while (estadoHilo[0]) {
        estadoHilo[1] = 0;
        // Cede turno al siguiente hilo
        while (turnoHilo != 1) {
            /* Espera su turno */
        }
        // Un tiempo de espera para que pase el siguiente
        estadoHilo[1] = 1;
    }

    // Entra a la seccion critica
    saldo -= 300;

    // Cambio de turno
    turnoHilo = 0;
    estadoHilo[1] = 0;

    // Resto del codigo
    return NULL;
}

int main() {
    printf("=== DEKKER V4 ===\n");
    printf("Saldo inicial: %d\n\n", saldo);

    /*
    Se crea la ejecución concurrente.
    La librería mutex se encarga de iniciar ambos hilos.
    */
    ThreadPair procesos = crear_procesos(proceso_0, proceso_1);

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
- Si hay conflicto, el hilo baja su bandera.
- Luego espera a que llegue su turno.
- Cuando el turno llega, vuelve a levantar la bandera.
- Esto mejora el manejo del conflicto y evita esperas innecesarias.
*/