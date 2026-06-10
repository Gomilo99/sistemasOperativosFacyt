// 10-banquero.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "mutex.h"
#include "semaphore.h"

// Número de procesos (hilos) y tipos de recursos
#define P 3
#define T 2

// Recursos totales de cada tipo (impresoras, scanners, etc.)
int total[T] = {10, 5};   // 10 unidades del recurso 0, 5 del recurso 1

// Estado actual del sistema
int asignado[P][T];
int max_nec[P][T];
int necesidad[P][T];
int disponible[T];

// Semáforo para proteger el estado global (mutex del banquero)
semaphore banquero_mutex;

// Inicialización del estado (valores de ejemplo)
void inicializar_estado() {
    // Inicialmente ningún recurso asignado
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < T; j++) {
            asignado[i][j] = 0;
            necesidad[i][j] = max_nec[i][j];   // inicialmente toda la necesidad
        }
    }
    // Recursos disponibles al inicio = total
    for (int j = 0; j < T; j++) {
        disponible[j] = total[j];
    }
}

// Verificar si el estado actual es seguro (algoritmo de seguridad)
int es_estado_seguro() {
    int work[T];
    int finish[P];
    // Copia de disponibles
    for (int j = 0; j < T; j++) work[j] = disponible[j];
    // Ningún proceso ha terminado
    for (int i = 0; i < P; i++) finish[i] = 0;

    int encontrado;
    do {
        encontrado = 0;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                // Verificar si necesidad[i] <= work
                int puede = 1;
                for (int j = 0; j < T; j++) {
                    if (necesidad[i][j] > work[j]) {
                        puede = 0;
                        break;
                    }
                }
                if (puede) {
                    // Simula que i termina y libera sus recursos
                    for (int j = 0; j < T; j++) work[j] += asignado[i][j];
                    finish[i] = 1;
                    encontrado = 1;
                }
            }
        }
    } while (encontrado);

    // Si todos terminaron, es seguro
    for (int i = 0; i < P; i++) {
        if (!finish[i]) return 0;
    }
    return 1;
}

// Solicitud de recursos por parte del proceso pid
// Devuelve 1 si se concede, 0 si no (debe esperar)
int solicitar_recursos(int pid, int request[T]) {
    wait(&banquero_mutex);
    
    // 1. ¿La solicitud excede la necesidad máxima?
    for (int j = 0; j < T; j++) {
        if (request[j] > necesidad[pid][j]) {
            printf("❌ Proceso %d: solicitud excede necesidad máxima. Rechazada.\n", pid);
            signal(&banquero_mutex);
            return 0;
        }
    }
    // 2. ¿Hay suficientes recursos disponibles?
    for (int j = 0; j < T; j++) {
        if (request[j] > disponible[j]) {
            printf("⏳ Proceso %d: recursos insuficientes. Debe esperar.\n", pid);
            signal(&banquero_mutex);
            return 0;
        }
    }
    
    // 3. Suponer que concedemos la solicitud (estado tentativo)
    for (int j = 0; j < T; j++) {
        disponible[j] -= request[j];
        asignado[pid][j] += request[j];
        necesidad[pid][j] -= request[j];
    }
    
    // 4. Verificar si el nuevo estado es seguro
    if (es_estado_seguro()) {
        printf("✅ Proceso %d: solicitud concedida. Nuevo estado seguro.\n", pid);
        signal(&banquero_mutex);
        return 1;
    } else {
        // Revertir los cambios (estado inseguro)
        for (int j = 0; j < T; j++) {
            disponible[j] += request[j];
            asignado[pid][j] -= request[j];
            necesidad[pid][j] += request[j];
        }
        printf("⚠️  Proceso %d: solicitud denegada (llevaría a estado inseguro).\n", pid);
        signal(&banquero_mutex);
        return 0;
    }
}

// Liberar recursos (cuando el proceso termina)
void liberar_recursos(int pid) {
    wait(&banquero_mutex);
    for (int j = 0; j < T; j++) {
        disponible[j] += asignado[pid][j];
        asignado[pid][j] = 0;
        necesidad[pid][j] = max_nec[pid][j];   // reiniciar necesidad
    }
    printf("🔄 Proceso %d: liberó todos sus recursos.\n", pid);
    signal(&banquero_mutex);
}

// Función que ejecuta cada hilo (proceso)
void* proceso(void* arg) {
    int pid = *(int*)arg;
    free(arg);
    
    // Ciclo de vida: solicita recursos, trabaja, libera
    for (int ciclo = 0; ciclo < 2; ciclo++) {
        // Dormir un tiempo aleatorio
        usleep(rand() % 500000);
        
        // Generar una solicitud aleatoria (no excede max_nec)
        int request[T];
        for (int j = 0; j < T; j++) {
            int max_solicitud = necesidad[pid][j];
            request[j] = (max_solicitud > 0) ? (rand() % (max_solicitud + 1)) : 0;
        }
        
        printf("📨 Proceso %d: solicita [%d, %d]\n", pid, request[0], request[1]);
        
        // Intentar obtener recursos
        if (solicitar_recursos(pid, request)) {
            // Simular uso de los recursos (trabajo)
            printf("💼 Proceso %d: trabajando con sus recursos...\n", pid);
            usleep(rand() % 800000);
            
            // Liberar recursos
            liberar_recursos(pid);
        } else {
            // Solicitud denegada: espera un poco y reintentará en el próximo ciclo
            printf("😴 Proceso %d: esperando para reintentar...\n", pid);
            usleep(rand() % 600000);
        }
    }
    printf("🏁 Proceso %d: terminó.\n", pid);
    return NULL;
}

int main() {
    srand(time(NULL));
    printf("=== ALGORITMO DEL BANQUERO (Banker's Algorithm) ===\n");
    printf("Recursos totales: R0=%d, R1=%d\n", total[0], total[1]);
    
    // Definir necesidad máxima de cada proceso (ejemplo)
    // Proceso 0: necesita max 7 de R0 y 3 de R1
    // Proceso 1: necesita max 3 de R0 y 2 de R1
    // Proceso 2: necesita max 9 de R0 y 2 de R1
    max_nec[0][0] = 7; max_nec[0][1] = 3;
    max_nec[1][0] = 3; max_nec[1][1] = 2;
    max_nec[2][0] = 9; max_nec[2][1] = 2;
    
    inicializar_estado();
    init(&banquero_mutex, 1);
    
    // Crear hilos
    ThreadArray procesos = crear_n_procesos(P, proceso);
    esperar_n_procesos(procesos);
    liberar_n_procesos(procesos);
    
    destroy(&banquero_mutex);
    printf("\n✅ Simulación finalizada.\n");
    return 0;
}