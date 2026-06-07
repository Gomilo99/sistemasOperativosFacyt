#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// VARIABLES COMPARTIDAS
int turnoHilo = 0;      // 0 = turno H0, 1 = turno H1
int totalHilos = 2;
int saldo = 1000;

// Función que ejecutará el hilo 0
void* Hilo0(void* arg) {
    // ESPERA A SU TURNO
    while (turnoHilo != 0) {
    }

    saldo -= 500;

    // LIBERA
    turnoHilo = 1;  // Ahora es turno de Hilo1

    return NULL;
}

// Función que ejecutará el hilo 1
void* Hilo1(void* arg) {
    printf("🟠 Hilo1: Iniciado\n");
    printf("🟠 Hilo1: Intentando entrar a sección crítica\n");
    
    // ESPERA A SU TURNO
    while (turnoHilo != 1) {
        printf("🟠 Hilo1: Esperando... (turno=%d)\n", turnoHilo);
        usleep(100000);  // Espera 100ms
    }
    
    // SECCIÓN CRÍTICA
    printf("🟠 Hilo1: ¡¡ENTRÉ!! Saldo actual: %d\n", saldo);
    saldo -= 300;
    printf("🟠 Hilo1: Retiré 300. Saldo nuevo: %d\n", saldo);
    
    // LIBERA
    turnoHilo = 0;  // Ahora es turno de Hilo0
    printf("🟠 Hilo1: Salí. turno=%d\n\n", turnoHilo);
    
    return NULL;
}

int main() {
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║  ALGORITMO DE DEKKER V1 CON THREADS          ║\n");
    printf("║  Ejecución CONCURRENTE (real)                ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    
    printf("Saldo inicial: %d\n");
    printf("Turno inicial: %d\n\n", turnoHilo);
    
    // DECLARAR DOS HILOS
    pthread_t hilo0, hilo1;
    
    // CREAR (INICIAR) LOS HILOS
    printf("⚙️  Creando hilos...\n");
    pthread_create(&hilo0, NULL, Hilo0, NULL);
    pthread_create(&hilo1, NULL, Hilo1, NULL);
    printf("✓ Hilos creados\n\n");
    
    // ESPERAR A QUE LOS HILOS TERMINEN
    printf("⏳ Esperando a que los hilos terminen...\n");
    pthread_join(hilo0, NULL);  // Espera a que hilo0 termine
    pthread_join(hilo1, NULL);  // Espera a que hilo1 termine
    printf("✓ Todos los hilos terminaron\n\n");
    
    // MOSTRAR RESULTADO
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║  RESULTADO FINAL                              ║\n");
    printf("║  Saldo: %d (Esperado: 200)                   ║\n", saldo);
    if (saldo == 200) {
        printf("║  ✓ CORRECTO - Exclusión mutua funcionó      ║\n");
    } else {
        printf("║  ✗ INCORRECTO - Problema de sincronización  ║\n");
    }
    printf("╚════════════════════════════════════════════════╝\n");
    
    return 0;
}