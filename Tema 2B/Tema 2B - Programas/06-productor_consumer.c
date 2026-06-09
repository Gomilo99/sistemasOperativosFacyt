// productor_consumidor_con_mi_sem.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mutex.h"
#include "semaphore.h"

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

semaphore sem_mutex;   // control de acceso al buffer (binario)
semaphore sem_empty;   // espacios vacíos (inicial = BUFFER_SIZE)
semaphore sem_full;    // elementos llenos (inicial = 0)

void* productor(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        printf("Productor: produce %d\n", i);
        usleep(rand() % 200000);

        wait(&sem_empty);   // espera un hueco
        wait(&sem_mutex);   // protege buffer

        buffer[in] = i;
        printf("Productor: inserta %d en pos %d\n", i, in);
        in = (in + 1) % BUFFER_SIZE;

        signal(&sem_mutex);
        signal(&sem_full);    // avisa que hay nuevo elemento
    }
    return NULL;
}

void* consumidor(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        wait(&sem_full);     // espera elemento
        wait(&sem_mutex);    // protege buffer

        int item = buffer[out];
        printf("Consumidor: toma %d de pos %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        signal(&sem_mutex);
        signal(&sem_empty);   // avisa que hay un hueco

        printf("Consumidor: consume %d\n", item);
        usleep(rand() % 200000);
    }
    return NULL;
}

int main() {
    printf("=== PRODUCTOR-CONSUMIDOR CON MI_SEM ===\n");
    init(&sem_mutex, 1);          // mutex binario
    init(&sem_empty, BUFFER_SIZE);
    init(&sem_full, 0);

    ThreadPair hilos = crear_procesos(productor, consumidor);
    esperar_procesos(hilos);

    destroy(&sem_mutex);
    destroy(&sem_empty);
    destroy(&sem_full);

    printf("\nTerminado.\n");
    return 0;
}