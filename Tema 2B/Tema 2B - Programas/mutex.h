#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

// Estructura para guardar info de los threads
typedef struct {
    pthread_t thread0;
    pthread_t thread1;
} ThreadPair;

// Crear y ejecutar dos procesos concurrentemente
ThreadPair crear_procesos(void* (*proceso_0)(void*), 
                          void* (*proceso_1)(void*));

// Esperar a que terminen
void esperar_procesos(ThreadPair pair);

#endif