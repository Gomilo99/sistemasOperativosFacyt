#include "mutex.h"
#include <stdio.h>

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