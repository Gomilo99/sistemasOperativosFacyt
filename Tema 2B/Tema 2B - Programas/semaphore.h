#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <pthread.h>

// Estructura que representa un semáforo
typedef struct {
    int value;              // valor del semáforo
    pthread_mutex_t mutex;  // protege el acceso a value
    pthread_cond_t cond;    // para bloquear/despertar hilos
} semaphore;

// Inicializar semáforo con un valor inicial (≥0)
void init(semaphore *sem, int valor_inicial);

// Operación wait (P): decrementa, o bloquea si value == 0
void wait(semaphore *sem);

// Operación signal (V): incrementa y despierta un hilo si hay
void signal(semaphore *sem);

// Destruir semáforo (liberar recursos internos)
void destroy(semaphore *sem);

#endif