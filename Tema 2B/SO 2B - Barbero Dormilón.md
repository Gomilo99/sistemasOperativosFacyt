---
creado: 10/06/2026
modificado: 10/06/2026
alias:
tags:
  - sistemas_operativos
  - estudio
tipo: Concepto
base:
  - "[[A-Sistemas Operativos]]"
  - "[[04-Recursos]]"
---
## Links
- [[SO - Sincronización]]
- [[SO - Algoritmo del Banquero]]
- [[SO - Filósofos comensales]]
- [[SO - Barbero Dormilón]]
## El Problema del Barbero Dormilón (Sleeping Barber)
### ¿Qué es el problema?
Es un problema clásico de sincronización que modela una barbería con:
- **Un barbero** que atiende clientes.
- **Una sala de espera** con `N` sillas (buffer acotado).
- **Clientes** que llegan en momentos aleatorios.
**Reglas:**
- Si no hay clientes, el barbero **duerme** en su silla.
- Si llega un cliente y el barbero duerme, lo despierta.
- Si llega un cliente y hay sillas libres en espera, espera sentado.
- Si llega un cliente y todas las sillas están ocupadas, se va (se pierde el cliente).
### ¿Qué problemas resuelve?

|Problema|Cómo lo resuelve|
|---|---|
|**Sincronización productor-consumidor**|Los clientes "producen" solicitudes, el barbero "consume".|
|**Control de recursos limitados**|Las sillas son el buffer acotado.|
|**Espera activa innecesaria**|El barbero se duerme (bloquea) cuando no hay trabajo.|
|**Exclusión mutua**|El acceso a contadores (clientes_esperando, sillas_ocupadas) debe ser protegido.|

### Implementación con tu librería
Usaremos los semáforos que ya tienes (`semaphore.h`) y tu `mutex.h` para la creación de hilos.
```c
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
        printf("💈 Barbero: esperando clientes...\n");
        
        wait(&cliente_listo);      // espera a que llegue un cliente
        wait(&mutex);              // protege clientes_esperando
        
        clientes_esperando--;      // el cliente pasa de la silla de espera a la silla del barbero
        printf("💈 Barbero: despierta! Clientes en espera: %d\n", clientes_esperando);
        
        signal(&mutex);
        signal(&barbero_listo);    // indica que el barbero está listo para atender
        
        // --- SECCIÓN CRÍTICA (ATENDER CLIENTE) ---
        printf("💈 Barbero: ✂️  Atendiendo cliente... (atendidos: %d)\n", ++clientes_atendidos);
        usleep(rand() % 300000 + 200000);  // atender toma tiempo
        
        printf("💈 Barbero: ✅ Cliente atendido\n");
    }
    return NULL;
}
void* cliente(void* arg) {
    int id = *(int*)arg;
    free(arg);
    
    printf("🚶 Cliente %d: llega a la barbería\n", id);
    
    wait(&mutex);  // protege clientes_esperando
    
    if (clientes_esperando < NUM_SILLAS) {
        // Hay silla disponible
        clientes_esperando++;
        printf("🚶 Cliente %d: toma asiento (esperando: %d)\n", id, clientes_esperando);
        
        signal(&cliente_listo);    // avisa al barbero que hay un cliente
        signal(&mutex);
        
        wait(&barbero_listo);      // espera a que el barbero lo atienda
        
        // --- SECCIÓN CRÍTICA (SER ATENDIDO) ---
        printf("🚶 Cliente %d: 💈 siendo atendido...\n", id);
        usleep(rand() % 100000);   // tiempo de atención percibido
        
        printf("🚶 Cliente %d: 🚪 sale de la barbería\n", id);
    } else {
        // No hay sillas disponibles, el cliente se va
        clientes_perdidos++;
        printf("🚶 Cliente %d: 😤 no hay sillas, me voy (perdidos: %d)\n", id, clientes_perdidos);
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

```
### Versión con tu librería `mutex.h` (crear_n_procesos)
```c
// 08-barbero_dormilon_v2.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mutex.h"
#include "semaphore.h"

#define NUM_SILLAS 3
#define NUM_CLIENTES 10

semaphore mutex;
semaphore barbero_listo;
semaphore cliente_listo;
int clientes_esperando = 0;
int clientes_atendidos = 0;
int clientes_perdidos = 0;

void* barbero(void* arg) {
    while (1) {
        printf("💈 Barbero: durmiendo... Zzz\n");
        
        wait(&cliente_listo);
        wait(&mutex);
        
        clientes_esperando--;
        printf("💈 Barbero: se levanta! Esperando: %d\n", clientes_esperando);
        
        signal(&mutex);
        signal(&barbero_listo);
        
        printf("💈 Barbero: ✂️  Atendiendo cliente (%d atendidos)\n", ++clientes_atendidos);
        usleep(rand() % 300000 + 200000);
        printf("💈 Barbero: ✅ Terminó\n");
    }
    return NULL;
}

void* cliente(void* arg) {
    int id = *(int*)arg;
    
    printf("🚶 Cliente %d: llega\n", id);
    
    wait(&mutex);
    
    if (clientes_esperando < NUM_SILLAS) {
        clientes_esperando++;
        printf("🚶 Cliente %d: se sienta (espera: %d/%d)\n", id, clientes_esperando, NUM_SILLAS);
        
        signal(&cliente_listo);
        signal(&mutex);
        
        wait(&barbero_listo);
        printf("🚶 Cliente %d: 💈 siendo atendido\n", id);
        usleep(rand() % 100000);
        printf("🚶 Cliente %d: 🚪 sale\n", id);
    } else {
        clientes_perdidos++;
        printf("🚶 Cliente %d: 😤 no hay sillas, se va (perdidos: %d)\n", id, clientes_perdidos);
        signal(&mutex);
    }
    
    return NULL;
}

int main() {
    printf("=== BARBERO DORMILÓN (con mutex.h) ===\n");
    printf("Sillas: %d | Clientes: %d\n\n", NUM_SILLAS, NUM_CLIENTES);
    
    srand(time(NULL));
    
    init(&mutex, 1);
    init(&barbero_listo, 0);
    init(&cliente_listo, 0);
    
    // Crear hilos usando crear_n_procesos (todos clientes)
    ThreadArray clientes_array = crear_n_procesos(NUM_CLIENTES, cliente);
    
    // Crear barbero manualmente (porque es un bucle infinito)
    pthread_t barbero_thread;
    pthread_create(&barbero_thread, NULL, barbero, NULL);
    
    // Esperar clientes
    esperar_n_procesos(clientes_array);
    liberar_n_procesos(clientes_array);
    
    // Esperar que termine el último cliente atendido
    sleep(3);
    pthread_cancel(barbero_thread);
    
    destroy(&mutex);
    destroy(&barbero_listo);
    destroy(&cliente_listo);
    
    printf("\n=== RESULTADOS ===\n");
    printf("Atendidos: %d\n", clientes_atendidos);
    printf("Perdidos: %d\n", clientes_perdidos);
    
    return 0;
}
```

### Problemas potenciales

|Problema|Descripción|Solución|
|---|---|---|
|**Inanición del barbero**|No ocurre, el barbero solo trabaja cuando hay clientes|-|
|**Pérdida de clientes**|Clientes se van si no hay sillas|Es parte del problema, modela situación real|
|**Condición de carrera**|En `clientes_esperando` si no se protege con mutex|Usamos `mutex` para protegerlo|
|**Deadlock**|Posible si se ordenan mal los semáforos|El orden es correcto: cliente hace `signal(cliente_listo)` antes de `wait(barbero_listo)`|

### Diferencia con productor-consumidor

|Aspecto|Productor-Consumidor|Barbero Dormilón|
|---|---|---|
|Productor|Cliente|Cliente|
|Consumidor|Barbero|Barbero|
|Buffer|Sillas de espera|Sillas de espera|
|Particularidad|El consumidor "duerme" cuando no hay trabajo|Misma idea, pero el barbero se bloquea|
|Clientes perdidos|No, productor espera|Sí, si buffer lleno se van|
## Corrida en frío resumida del Barbero Dormilón
### Configuración
- **Sillas de espera:** 2
- **Clientes:** 3 (C1, C2, C3)
- **Barbero:** 1 (B)
### Estado inicial
```text
clientes_esperando = 0
mutex = 1 (libre)
cliente_listo = 0 (no hay clientes)
barbero_listo = 0 (barbero no está listo)
Barbero: durmiendo (bloqueado en wait(&cliente_listo))
```
### Secuencia paso a paso

|Paso|Hilo|Operación|clientes_esperando|cliente_listo|barbero_listo|Estado|
|---|---|---|---|---|---|---|
|0|-|Inicial|0|0|0|Barbero duerme|
|1|**C1**|`wait(mutex)` → adquiere|0|0|0||
|2|C1|¿2 sillas? Sí → `clientes_esperando = 1`|1|0|0|C1 se sienta|
|3|C1|`signal(cliente_listo)`|1|**1**|0|**Despierta barbero**|
|4|C1|`signal(mutex)`|1|1|0|Libera mutex|
|5|C1|`wait(barbero_listo)` → **se bloquea**|1|1|0|Espera ser atendido|
|6|**B**|`wait(cliente_listo)` → **se despierta**|1|0|0|Barbero se levanta|
|7|B|`wait(mutex)`|1|0|0||
|8|B|`clientes_esperando = 0`|0|0|0|C1 pasa al barbero|
|9|B|`signal(mutex)`|0|0|0||
|10|B|`signal(barbero_listo)`|0|0|**1**|Barbero listo|
|11|B|**✂️ Atiende a C1**|0|0|1||
|12|**C1**|`wait(barbero_listo)` → **despierta**|0|0|0|C1 es atendido|
|13|**C2**|Llega, `wait(mutex)`|0|0|0||
|14|C2|`clientes_esperando = 1`|1|0|0|C2 se sienta|
|15|C2|`signal(cliente_listo)`|1|**1**|0|Avisa al barbero|
|16|C2|`signal(mutex)`|1|1|0||
|17|C2|`wait(barbero_listo)` → **se bloquea**|1|1|0||
|18|**B**|Termina con C1|1|1|0||
|19|B|`wait(cliente_listo)` → sigue|1|0|0||
|20|B|`clientes_esperando = 0`|0|0|0||
|21|B|`signal(barbero_listo)`|0|0|1||
|22|B|**✂️ Atiende a C2**|0|0|1||
|23|**C2**|Despierta, es atendido|0|0|0||
|24|**C3**|Llega, ve 2 sillas libres|1|0|0|C3 se sienta|
|25|C3|`signal(cliente_listo)`|1|**1**|0||
|26|C3|`wait(barbero_listo)` → se bloquea|1|1|0||
|...|...|(el ciclo continúa)|...|...|...||

### Diagrama de estados simplificado
```text
INICIO: Barbero DURMIENDO
    │
    ▼
C1 llega → despierta Barbero → C1 se sienta (espera=1)
    │
    ▼
Barbero atiende C1 → C1 sale
    │
    ▼
C2 llega → se sienta (espera=1) → Barbero atiende C2
    │
    ▼
C3 llega mientras Barbero atiende → se sienta (espera=1)
    │
    ▼
Barbero termina C2 → atiende C3
    │
    ▼
Si no hay más clientes → Barbero VUELVE A DORMIR
```
### Conclusión de la corrida

|Evento|Resultado|
|---|---|
|**Cliente que llega con silla libre**|Se sienta y espera (o despierta al barbero si estaba dormido)|
|**Cliente que llega sin sillas**|Se pierde (no ocurre en esta corrida porque 3 clientes ≤ 2 sillas + 1 en atención)|
|**Barbero sin clientes**|Se bloquea (duerme) en `wait(cliente_listo)`|
|**Cliente siendo atendido**|Se bloquea en `wait(barbero_listo)` hasta que termine el actual|
|**Exclusión mutua**|`mutex` protege `clientes_esperando` de condiciones de carrera|

**El barbero duerme cuando no hay trabajo y se despierta automáticamente cuando llega un cliente.**