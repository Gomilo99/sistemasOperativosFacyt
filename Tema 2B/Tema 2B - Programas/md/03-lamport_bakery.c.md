// 03-lamport_bakery.c
#include <stdio.h>
#include "mutex.h"

// Variables compartidas
int choosing[2] = {0, 0};   // Indica si el hilo está eligiendo número
int number[2]   = {0, 0};   // Número de ticket asignado a cada hilo
int saldo = 1000;           // Recurso compartido

// Protocolo de entrada para el hilo i (i = 0 o 1)
void entrada(int i) {
    int j;
    // Paso 1: elegir un número de ticket
    choosing[i] = 1;
    number[i] = 1 + (number[0] > number[1] ? number[0] : number[1]);
    choosing[i] = 0;
    // Paso 2: esperar a que todos los hilos con ticket menor terminen
    for (j = 0; j < 2; j++) {
        // Esperar si el otro hilo está eligiendo
        while (choosing[j]) {
            // espera activa
        }
        // Esperar si el otro hilo tiene ticket y su ticket es menor,
        // o si es igual pero su índice es menor (para romper simetría)
        while (number[j] != 0 && 
                (number[j] < number[i] ||
                (number[j] == number[i] && j < i))) {
            // espera activa
        }
    }
}

// Protocolo de salida para el hilo i
void salida(int i) {
    number[i] = 0;   // Libera el ticket
}

// Hilo 0: retira 500 del saldo
void* hilo_0(void* arg) {
    // Sección no crítica (código que no toca datos compartidos)
    entrada(0);          // Protocolo de entrada

    // Sección crítica
    saldo -= 500;
    printf("Hilo 0 ejecutó SC. Saldo: %d\n", saldo);

    salida(0);           // Protocolo de salida
    // Resto del código
    return NULL;
}

// Hilo 1: retira 300 del saldo
void* hilo_1(void* arg) {
    // Sección no crítica
    entrada(1);          // Protocolo de entrada

    // Sección crítica
    saldo -= 300;
    printf("Hilo 1 ejecutó SC. Saldo: %d\n", saldo);

    salida(1);          // Protocolo de salida
    return NULL;
}

int main() {
    printf("=== LAMPORT BAKERY ALGORITHM ===\n");
    printf("Saldo inicial: %d\n\n", saldo);

    // Crear y lanzar los dos hilos mediante la librería mutex
    ThreadPair hilos = crear_procesos(hilo_0, hilo_1);

    // Esperar a que ambos terminen
    esperar_procesos(hilos);

    printf("\nSaldo final esperado: 200\n");
    printf("Saldo final obtenido: %d\n", saldo);
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