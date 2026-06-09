// 08-barbero_dormilon.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mutex.h"
#include "semaphore.h"

#define NUM_SILLAS 3      // sillas en la sala de espera
#define NUM_CLIENTES 10   // clientes que llegan (para el ejemplo)

// Recursos compartidos
semaphore mutex;          // protege el contador de clientes_esperando
semaphore barbero_listo;  // indica si el barbero está listo para atender (0 = durmiendo, 1 = disponible)
semaphore cliente_listo;  // indica si hay clientes esperando (0 = no hay, >0 = hay)
int clientes_esperando = 0;  // clientes en sillas de espera

// Estadísticas
int clientes_atendidos = 0;
int clientes_perdidos = 0;

void* barbero(void* arg) {
    while (1) {
        wait(&cliente_listo);      // espera a que llegue un cliente
        wait(&mutex);              // protege clientes_esperando
        
        clientes_esperando--;      // el cliente pasa de la silla de espera a la silla del barbero
        signal(&mutex);
        signal(&barbero_listo);    // indica que el barbero está listo para atender
        
        // --- SECCIÓN CRÍTICA (ATENDER CLIENTE) ---
        usleep(rand() % 300000 + 200000);  // atender toma tiempo
        
        printf("💈 Barbero: ✅ Cliente atendido\n");
    }
    return NULL;
}

void* cliente(void* arg) {
    int id = *(int*)arg;
    free(arg);
    
    wait(&mutex);  // protege clientes_esperando
    
    if (clientes_esperando < NUM_SILLAS) {
        // Hay silla disponible
        clientes_esperando++;

        signal(&cliente_listo);    // avisa al barbero que hay un cliente
        signal(&mutex);
        
        wait(&barbero_listo);      // espera a que el barbero lo atienda
        
        // --- SECCIÓN CRÍTICA (SER ATENDIDO) ---
        usleep(rand() % 100000);   // tiempo de atención percibido
        
        printf("🚶 Cliente %d: 🚪 sale de la barbería\n", id);
    } else {
        // No hay sillas disponibles, el cliente se va
        clientes_perdidos++;
        signal(&mutex);
    }
    
    return NULL;
}

int main() {
    printf("=== PROBLEMA DEL BARBERO DORMILÓN ===\n");
    printf("Sillas disponibles: %d\n", NUM_SILLAS);
    printf("Clientes que llegarán: %d\n\n", NUM_CLIENTES);
    
    srand(time(NULL));
    
    // Inicializar semáforos
    init(&mutex, 1);
    init(&barbero_listo, 0);
    init(&cliente_listo, 0);
    
    // Crear hilo del barbero
    pthread_t barbero_thread;
    pthread_create(&barbero_thread, NULL, barbero, NULL);
    
    // Crear hilos de clientes (llegan en momentos aleatorios)
    pthread_t clientes_threads[NUM_CLIENTES];
    for (int i = 0; i < NUM_CLIENTES; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&clientes_threads[i], NULL, cliente, id);
        usleep(rand() % 500000 + 100000);  // los clientes llegan con intervalo aleatorio
    }
    
    // Esperar a que todos los clientes terminen
    for (int i = 0; i < NUM_CLIENTES; i++) {
        pthread_join(clientes_threads[i], NULL);
    }
    
    // Esperar un poco para que el barbero termine de atender
    sleep(2);
    
    // Nota: El barbero es un bucle infinito, lo cancelamos (en un programa real se manejaría con una condición de salida)
    pthread_cancel(barbero_thread);
    
    // Limpiar recursos
    destroy(&mutex);
    destroy(&barbero_listo);
    destroy(&cliente_listo);
    
    printf("\n=== ESTADÍSTICAS FINALES ===\n");
    printf("Clientes atendidos: %d\n", clientes_atendidos);
    printf("Clientes perdidos: %d\n", clientes_perdidos);
    printf("Total clientes: %d\n", clientes_atendidos + clientes_perdidos);
    
    return 0;
}