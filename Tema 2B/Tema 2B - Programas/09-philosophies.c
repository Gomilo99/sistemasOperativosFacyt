// 09-filosofos.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mutex.h"
#include "semaphore.h"

#define N 5                     // número de filósofos
#define PENSANDO  0
#define HAMBRIENTO 1
#define COMIENDO  2

semaphore tenedor[N];           // un semáforo por tenedor (1 = libre)
semaphore limite;               // permite solo N-1 filósofos a la vez
semaphore print_mutex;          // para ordenar la salida en pantalla

void pensar(int id) {
    printf("Filósofo %d está PENSANDO\n", id);
    usleep(rand() % 800000);
}

void comer(int id) {
    printf("Filósofo %d está COMIENDO  🍝\n", id);
    usleep(rand() % 400000);
}

void* filosofo(void* arg) {
    int i = *(int*)arg;
    free(arg);
    
    while (1) {                     // ciclo infinito (filósofo activo)
        pensar(i);
        
        wait(&limite);              // solo N-1 filósofos pueden pasar
        
        wait(&tenedor[i]);          // toma tenedor izquierdo
        wait(&tenedor[(i+1) % N]);  // toma tenedor derecho
        
        comer(i);
        
        signal(&tenedor[i]);        // suelta izquierdo
        signal(&tenedor[(i+1) % N]); // suelta derecho
        
        signal(&limite);            // libera cupo
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    printf("=== FILÓSOFOS COMENSALES ===\n");
    printf("N = %d filósofos\n", N);
    
    init(&limite, N-1);            // máximo N-1 a la vez
    init(&print_mutex, 1);
    for (int i = 0; i < N; i++)
        init(&tenedor[i], 1);
    
    ThreadArray filosofos = crear_n_procesos(N, filosofo);
    
    // Nunca termina a menos que se fuerce (ejemplo didáctico)
    esperar_n_procesos(filosofos);
    
    // Limpieza (no se alcanza en este código)
    liberar_n_procesos(filosofos);
    for (int i = 0; i < N; i++) destroy(&tenedor[i]);
    destroy(&limite);
    destroy(&print_mutex);
    
    return 0;
}