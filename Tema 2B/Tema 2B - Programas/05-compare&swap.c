// 05-compare_and_swap.c
#include <stdio.h>
#include "mutex.h"

// Variable compartida
volatile int lock = 0;
int saldo = 1000;

// Implementación de CAS usando built-in de GCC (atómico real)
int CompareAndSwap(volatile int *target, int expected, int new_value) {
    // __sync_val_compare_and_swap es atómico y retorna el valor original
    return __sync_val_compare_and_swap(target, expected, new_value);
}

int CompareAndSwap(int *target, int expected, int new_value) {
    int old = *target;           // Lee el valor actual
    if (old == expected) {       // Si coincide con lo esperado
        *target = new_value;     // Actualiza al nuevo valor
    }
    return old;                  // Devuelve el valor original
}

void* hilo_0(void* arg) {
    // Protocolo de entrada con CAS
    while (1) {
        // Si lock es 0, intentamos cambiarlo a 1
        if (CompareAndSwap(&lock, 0, 1) == 0) {
            break;  // Logró adquirir el lock
        }
        // Si no, sigue esperando (spinning)
    }
    // --- SECCIÓN CRÍTICA ---
    saldo -= 500;

    // Protocolo de salida
    lock = 0;
    return NULL;
}

void* hilo_1(void* arg) {
    while (1) {
        if (CompareAndSwap(&lock, 0, 1) == 0) {
            break;
        }
    }
    saldo -= 300;

    lock = 0;
    return NULL;
}

int main() {
    printf("=== COMPARE & SWAP (CAS) ALGORITHM ===\n");
    printf("Usando __sync_val_compare_and_swap (atómico real del CPU)\n");
    printf("Saldo inicial: %d\n\n", saldo);
    
    ThreadPair hilos = crear_procesos(hilo_0, hilo_1);
    esperar_procesos(hilos);
    
    printf("\nSaldo final: %d\n", saldo);
    printf("Resultado esperado: %d\n", 1000 - 500 - 300);
    
    return 0;
}