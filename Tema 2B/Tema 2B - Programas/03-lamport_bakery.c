// lamport_bakery_n_mutex.c
#include <stdio.h>
#include "mutex.h"

// Número de hilos
#define NUM_THREADS 5
// Cantidad que resta cada hilo
#define CANTIDAD_RESTA 100

// Variables compartidas
int choosing[NUM_THREADS];
int number[NUM_THREADS];
int saldo = 1000;

// Máximo de dos enteros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Protocolo de entrada para el hilo i
void entrada(int i) {
    int j;
    choosing[i] = 1;
    // Obtener el ticket máximo actual
    int max_number = 0;
    for (j = 0; j < NUM_THREADS; j++) {
        max_number = max(max_number, number[j]);
    }
    number[i] = max_number + 1;
    choosing[i] = 0;

    // Esperar a todos los hilos con ticket menor
    for (j = 0; j < NUM_THREADS; j++) {
        while (choosing[j]);   // esperar a que j termine de elegir
        while (number[j] != 0 &&
               (number[j] < number[i] ||
                (number[j] == number[i] && j < i))) {
            // espera activa
        }
    }
}

// Protocolo de salida
void salida(int i) {
    number[i] = 0;
}

// Función que ejecuta cada hilo
void* hilo(void* arg) {
    int i = *(int*)arg;   // índice del hilo
    free(arg);            // liberar la memoria reservada en crear_n_procesos

    // Sección crítica
    entrada(i);
    // SC: restar una cantidad fija al saldo
    if (saldo >= CANTIDAD_RESTA) {
        printf("Hilo %d resta %d. Saldo antes: %d\n", i, CANTIDAD_RESTA, saldo);
        saldo -= CANTIDAD_RESTA;
        printf("Hilo %d termina SC. Saldo ahora: %d\n", i, saldo);
    } else {
        printf("Hilo %d no pudo restar (saldo insuficiente: %d)\n", i, saldo);
    }
    salida(i);

    return NULL;
}

int main() {
    printf("=== ALGORITMO DE LAMPORT (PANADERÍA) PARA %d HILOS ===\n", NUM_THREADS);
    printf("Saldo inicial: %d\n", saldo);
    printf("Cada hilo resta %d\n\n", CANTIDAD_RESTA);

    // Crear los hilos usando la librería mutex extendida
    ThreadArray hilos = crear_n_procesos(NUM_THREADS, hilo);

    // Esperar a que terminen
    esperar_n_procesos(hilos);

    // Liberar recursos
    liberar_n_procesos(hilos);

    printf("\nSaldo final: %d\n", saldo);
    printf("Saldo esperado: %d\n", 1000 - NUM_THREADS * CANTIDAD_RESTA);
    return 0;
}

/*
CORRIDA EN FRÍO:

Estado inicial:
saldo = 1000
choosing[0] = 0, choosing[1] = 0
number[0] = 0, number[1] = 0

Posible ejecución (sin interleaving conflictivo):

Hilo 0:
- entrada(0): choosing[0]=1, number[0] = 1+max(0,0)=1, choosing[0]=0.
- Bucle for j=0..1:
   j=0: choosing[0] ya es 0, number[0]=1, pero compara con sí mismo (j==i), la condición (number[0] < number[0] es falsa, y number[0]==number[0] && j<i -> 0<0 falso), no espera.
   j=1: choosing[1]=0, number[1]=0, entonces no espera.
- Entra a SC: saldo = 1000 - 500 = 500.
- salida(0): number[0]=0.

Hilo 1 (se ejecuta después, o concurrentemente pero con tickets asignados):
- entrada(1): choosing[1]=1, number[1] = 1+max(1,0)=2, choosing[1]=0.
- Bucle for:
   j=0: choosing[0]=0, number[0]=0 -> no espera.
   j=1: se saltea a sí mismo.
- Entra a SC: saldo = 500 - 300 = 200.
- salida(1): number[1]=0.

Saldo final: 200.

Explicación:
- El algoritmo de la panadería garantiza exclusión mutua, ausencia de deadlock y
  equidad (fairness) mediante la asignación de tickets ordenados.
- Si ambos hilos intentan entrar casi al mismo tiempo, el que obtiene el ticket
  menor (o mismo ticket con menor índice) accede primero.
- La variable 'choosing' evita condiciones de carrera durante la asignación
  del ticket.
*/