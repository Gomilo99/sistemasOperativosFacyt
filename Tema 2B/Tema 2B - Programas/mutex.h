#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

// Estructura para dos hilos (compatibilidad con ejercicios anteriores)
typedef struct {
    pthread_t thread0;
    pthread_t thread1;
} ThreadPair;

// Estructura para N hilos
typedef struct {
    pthread_t* threads;   // Arreglo de identificadores de hilos
    int count;            // Número de hilos
} ThreadArray;

// Funciones para dos hilos (como antes)
ThreadPair crear_procesos(void* (*proceso_0)(void*),
                          void* (*proceso_1)(void*));
void esperar_procesos(ThreadPair pair);

// Nuevas funciones para N hilos
ThreadArray crear_n_procesos(int n, void* (*proceso)(void*));
void esperar_n_procesos(ThreadArray array);
void liberar_n_procesos(ThreadArray array);  // Libera la memoria interna

#endif