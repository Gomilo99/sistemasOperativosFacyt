#include "mutex.h"
#include <stdio.h>
#include <stdlib.h>

// --- Funciones para dos hilos (sin cambios) ---
ThreadPair crear_procesos(void* (*proceso_0)(void*),
                          void* (*proceso_1)(void*)) {
    ThreadPair pair;
    pthread_create(&pair.thread0, NULL, proceso_0, NULL);
    pthread_create(&pair.thread1, NULL, proceso_1, NULL);
    return pair;
}

void esperar_procesos(ThreadPair pair) {
    pthread_join(pair.thread0, NULL);
    pthread_join(pair.thread1, NULL);
}

// --- Nuevas funciones para N hilos ---
ThreadArray crear_n_procesos(int n, void* (*proceso)(void*)) {
    ThreadArray array;
    array.count = n;
    array.threads = (pthread_t*)malloc(n * sizeof(pthread_t));
    if (array.threads == NULL) {
        perror("malloc");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        // Se pasa el índice i al hilo (se usará como argumento)
        int* idx = (int*)malloc(sizeof(int));
        *idx = i;
        if (pthread_create(&array.threads[i], NULL, proceso, (void*)idx) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    return array;
}

void esperar_n_procesos(ThreadArray array) {
    for (int i = 0; i < array.count; i++) {
        pthread_join(array.threads[i], NULL);
    }
}

void liberar_n_procesos(ThreadArray array) {
    free(array.threads);
}