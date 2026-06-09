// 07-lectores_escritores.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mutex.h"
#include "semaphore.h"

// Recurso compartido
int datos_compartidos = 0;

// Variables de sincronización
semaphore mutex_lectores;   // protege contador de lectores
semaphore recurso;          // controla acceso al recurso (para escritores)
int lectores_activos = 0;   // cuántos lectores están leyendo

void* lector(void* arg) {
    int id = *(int*)arg;
    free(arg);
    
    for (int i = 0; i < 3; i++) {
        usleep(rand() % 100000);
        
        // --- PROTOCOLO DE ENTRADA (LECTOR) ---
        wait(&mutex_lectores);
        lectores_activos++;
        if (lectores_activos == 1) {
            wait(&recurso);  // primer lector bloquea el recurso para escritores
        }
        signal(&mutex_lectores);
        
        // --- SECCIÓN CRÍTICA (LECTURA) ---
        printf("📖 Lector %d: leyendo datos = %d (lectores activos: %d)\n", 
               id, datos_compartidos, lectores_activos);
        usleep(rand() % 50000);  // simula tiempo de lectura
        
        // --- PROTOCOLO DE SALIDA (LECTOR) ---
        wait(&mutex_lectores);
        lectores_activos--;
        if (lectores_activos == 0) {
            signal(&recurso);  // último lector libera el recurso
        }
        signal(&mutex_lectores);
    }
    return NULL;
}

void* escritor(void* arg) {
    int id = *(int*)arg;
    free(arg);
    
    for (int i = 0; i < 3; i++) {
        usleep(rand() % 200000);
        
        // --- PROTOCOLO DE ENTRADA (ESCRITOR) ---
        wait(&recurso);  // espera acceso exclusivo al recurso
        
        // --- SECCIÓN CRÍTICA (ESCRITURA) ---
        datos_compartidos++;
        printf("✍️  Escritor %d: escribiendo datos = %d (lectores activos: %d)\n", 
               id, datos_compartidos, lectores_activos);
        usleep(rand() % 100000);  // simula tiempo de escritura
        
        // --- PROTOCOLO DE SALIDA (ESCRITOR) ---
        signal(&recurso);
    }
    return NULL;
}

int main() {
    printf("=== LECTORES Y ESCRITORES (Prioridad a lectores) ===\n");
    printf("Usando semáforos y mutex.h para creación de hilos\n\n");
    
    // Inicializar semáforos
    init(&mutex_lectores, 1);
    init(&recurso, 1);
    
    // Crear array de hilos (3 lectores + 2 escritores = 5 hilos)
    ThreadArray hilos;
    hilos.count = 5;
    hilos.threads = malloc(5 * sizeof(pthread_t));
    
    // Crear lectores (ids 0, 1, 2)
    for (int i = 0; i < 3; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos.threads[i], NULL, lector, id);
    }
    
    // Crear escritores (ids 0, 1)
    for (int i = 0; i < 2; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos.threads[3 + i], NULL, escritor, id);
    }
    
    // Esperar a todos los hilos
    for (int i = 0; i < 5; i++) {
        pthread_join(hilos.threads[i], NULL);
    }
    
    // Limpiar recursos
    destroy(&mutex_lectores);
    destroy(&recurso);
    free(hilos.threads);
    
    printf("\n✅ Valor final de datos_compartidos: %d\n", datos_compartidos);
    printf("Esperado: %d (2 escritores × 3 escrituras = 6)\n", 6);
    
    return 0;
}