#include <stdio.h>
#include "mutex.h"

// VARIABLES COMPARTIDAS
int turnoHilo = 0;
int totalHilos = 2;
int estadoHilo[2] = {0, 0};
int saldo = 1000;

/*
Version 1: Alternancia simple.
Cada hilo espera a que el turno le corresponda.
*/
void* proceso_0(void* arg) {
    // Código que no afecta datos compartidos
    while (turnoHilo != 0) {
        /* Espera su turno */
    }
    // Entra a la sección crítica
    saldo -= 500;
    // Cambio de turno
    turnoHilo = (turnoHilo + 1) % totalHilos;
    // Resto del código
    return NULL;
}

void* proceso_1(void* arg) {
    // Código que no afecta datos compartidos
    while (turnoHilo != 1) {
        /* Espera su turno */
    }
    // Entra a la sección crítica
    saldo -= 300;
    // Cambio de turno
    turnoHilo = (turnoHilo + 1) % totalHilos;
    // Resto del código
    return NULL;
}

int main() {
    ThreadPair procesos = crear_procesos(proceso_0, proceso_1);
    esperar_procesos(procesos);
    return 0;
}

/*
CORRIDA EN FRÍO:

Estado inicial:
saldo = 1000
turnoHilo = 0

Hilo 0:
- turnoHilo = 0, entra
- saldo = 1000 - 500 = 500
- turnoHilo = 1

Hilo 1:
- turnoHilo = 1, entra
- saldo = 500 - 300 = 200
- turnoHilo = 0

Saldo final:
200

Explicación:
- Usa alternancia simple.
- El problema es que obliga a turnarse aunque un hilo no quiera entrar.
*/
