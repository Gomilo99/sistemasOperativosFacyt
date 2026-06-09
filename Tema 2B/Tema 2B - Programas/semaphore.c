#include "semaphore.h"

void init(semaphore *sem, int valor_inicial) {
    sem->value = valor_inicial;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

void wait(semaphore *sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->value == 0) {           // Si es cero, esperar
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->value--;                       // Decrementar atómicamente
    pthread_mutex_unlock(&sem->mutex);
}

void signal(semaphore *sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->value++;                       // Incrementar
    pthread_cond_signal(&sem->cond);    // Despertar un hilo (si hay)
    pthread_mutex_unlock(&sem->mutex);
}

void destroy(semaphore *sem) {
    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->cond);
}