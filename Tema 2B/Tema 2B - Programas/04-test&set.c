// 04-test_and_set.c
#include <stdio.h>
#include "mutex.h"

// Variable compartida para el lock
// Usamos volatile para evitar optimizaciones del compilador
volatile int lock = 0;

// Recurso compartido
int saldo = 1000;

// Simulación de Test&Set atómico
// En hardware real, esto sería una instrucción única
// En GCC se puede usar __sync_lock_test_and_set
int TestAndSet(volatile int *target) {
    // En un sistema real, esto sería atómico
    // Usamos built-in de GCC para atomicidad real
    return __sync_lock_test_and_set(target, 1);
}

int TestAndSet(int *target) {
    int old = *target;  // Lee el valor actual
    *target = 1;        // Establece a 1
    return old;         // Devuelve el valor original
}

void* hilo_0(void* arg) {
    // Sección no crítica
    
    // Protocolo de entrada (espera activa)
    while (TestAndSet(&lock) == 1) {
        // Spinning: espera activa hasta que el lock esté libre
    }
    // --- SECCIÓN CRÍTICA ---
    saldo -= 500;

    // Protocolo de salida
    lock = 0;  // Liberar el lock
    return NULL;
}

void* hilo_1(void* arg) {
    // Sección no crítica
    
    // Protocolo de entrada
    while (TestAndSet(&lock) == 1) {
        // Espera activa
    }
    // --- SECCIÓN CRÍTICA ---
    saldo -= 300;
    
    // Protocolo de salida
    lock = 0;
    return NULL;
}

int main() {
    printf("=== TEST & SET (TAS) ALGORITHM ===\n");
    printf("Saldo inicial: %d\n\n", saldo);
    
    ThreadPair hilos = crear_procesos(hilo_0, hilo_1);
    esperar_procesos(hilos);
    
    printf("\nSaldo final: %d\n", saldo);
    printf("Saldo esperado: %d\n", 1000 - 500 - 300);
    
    return 0;
}