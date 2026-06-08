#include <stdio.h>
#include <stdbool.h>
#include "mutex.h" // Uso de tu librería para manejo de hilos [2]

// VARIABLES COMPARTIDAS
bool flag[3] = {false, false};
int turn = 0;
int saldo = 1000;

// Hilo 0: Representa al primer proceso
void* hilo_0(void* arg) {
    // --- SECCIÓN DE ENTRADA ---
    flag[0] = true;
    turn = 1;
    while (flag[1] && turn == 1);

    // --- SECCIÓN CRÍTICA ---
    saldo -= 500;
    printf("Hilo 0 ejecutó SC. Saldo: %d\n", saldo);

    // --- SECCIÓN DE SALIDA ---
    flag[0] = false;
    return NULL;
}

// Hilo 1: Representa al segundo proceso
void* hilo_1(void* arg) {
    // --- SECCIÓN DE ENTRADA ---
    flag[1] = true;
    turn = 0;
    while (flag[0] && turn == 0);

    // --- SECCIÓN CRÍTICA ---
    saldo -= 300;
    printf("Hilo 1 ejecutó SC. Saldo: %d\n", saldo);

    // --- SECCIÓN DE SALIDA ---
    flag[1] = false;
    return NULL;
}

int main() {
    printf("=== ALGORITMO PETERSON ===\n");
    printf("Saldo inicial: %d\n\n", saldo);

    // Uso de la librería personalizada para lanzar los hilos
    ThreadPair hilos = crear_procesos(hilo_0, hilo_1);

    // Uso de la librería personalizada para esperar la finalización
    esperar_procesos(hilos);

    printf("\nSaldo final esperado: 200\n");
    printf("Saldo final obtenido: %d\n", saldo);

    return 0;
}