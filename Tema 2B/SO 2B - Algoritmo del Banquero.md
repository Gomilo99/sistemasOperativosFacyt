---
creado: 10/06/2026
modificado: 10/06/2026
alias:
tags:
  - sistemas_operativos
  - estudio
tipo: Concepto
base:
  - "[[04-Recursos]]"
  - "[[A-Sistemas Operativos]]"
---
## Links
- [[SO - Sincronización]]
- [[SO - Algoritmo del Banquero]]
- [[SO - Filósofos comensales]]
- [[SO - Barbero Dormilón]]
## Algoritmo del Banquero (Banker's Algorithm) – Prevención de Deadlock
El algoritmo del banquero, propuesto por Edger Dijkstra, es un algoritmo de **evitación de deadlocks** para sistemas con recursos múltiples. Su nombre proviene de la analogía con un banquero que concede préstamos a sus clientes asegurándose de que nunca se quede sin efectivo.
### ¿Qué problema resuelve?
En sistemas donde múltiples procesos compiten por varios tipos de recursos (impresoras, unidades de cinta, memoria, etc.), el algoritmo determina si una solicitud de recursos puede ser concedida inmediatamente o debe esperar para no caer en un **estado inseguro** (que podría llevar a deadlock).
### Principios fundamentales
1. **Recursos totales** – cantidad disponible de cada tipo de recurso.
2. **Asignados** – recursos actualmente en manos de cada proceso.
3. **Necesidad máxima** – lo máximo que cada proceso declaró que podría necesitar.
4. **Disponibles** – recursos libres en cada momento.
5. **Estado seguro** – existe al menos una secuencia de ejecución en la que todos los procesos pueden terminar sin deadlock.

El algoritmo del banquero opera bajo la premisa de que **los procesos deben declarar por adelantado su necesidad máxima de recursos**.
### Estructura de datos
- `int total_recursos[t]` – vector de recursos totales del sistema (para cada tipo).
- `int asignado[p][t]` – matriz de recursos asignados actualmente.
- `int max_nec[p][t]` – matriz de necesidad máxima declarada.
- `int necesidad[p][t]` – necesidad restante (`max_nec - asignado`).
- `int disponible[t]` – recursos disponibles.
### Implementación en C con threads
Para simular el comportamiento, creamos varios hilos (procesos) que periódicamente solicitan y liberan recursos. Un **banquero** central (representado por funciones protegidas por semáforos) decide si concede la solicitud usando el algoritmo de seguridad.
#### Archivos necesarios
- Usamos `mutex.h` para crear hilos (Tu librería).
- Usamos `semaphore.h` para proteger el estado global.
- No se requieren otras librerías externas.
#### Código completo
```c
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
```
### Corrida en frío (ejemplo concreto)
Supongamos el estado inicial:
- `total = [10, 5]`
- `max_nec` como arriba.
- Inicialmente `asignado = 0`, `necesidad = max_nec`, `disponible = [10,5]`.
**Caso 1: Solicitud segura – Proceso 1 pide [1,0]**

| Paso | Proceso                                                                                                                                       | Acción                          | `disponible` | `asignado[1]` | `necesidad[1]` | ¿Seguro?                              |
| ---- | --------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------- | ------------ | ------------- | -------------- | ------------------------------------- |
| 0    | –                                                                                                                                             | Inicio                          | [10,5]       | [0,0]         | [3,2]          | Sí                                    |
| 1    | P1                                                                                                                                            | Solicita [1,0]                  |              |               |                | Verifica: 1≤3, 0≤2, y 1≤10,0≤5 → pasa |
| 2    | Banquero                                                                                                                                      | Prueba concesión                | [9,5]        | [1,0]         | [2,2]          |                                       |
| 3    | Banquero                                                                                                                                      | Ejecuta algoritmo de seguridad: |              |               |                |                                       |
|      | Secuencia posible: P1 (necesita [2,2] ≤ [9,5] → termina, libera [1,0] → work=[10,5]), luego P0 (necesita [7,3] ≤ [10,5] → termina), luego P2. |                                 |              |               |                | Estado seguro                         |
| 4    | Banquero                                                                                                                                      | **Concede solicitud**           | [9,5]        | [1,0]         | [2,2]          |                                       |

**Caso 2: Solicitud insegura – Proceso 0 pide [8,0] (demasiado)**

|Paso|Proceso|Acción|`disponible`|`asignado[0]`|`necesidad[0]`|¿Seguro?|
|---|---|---|---|---|---|---|
|0|–|Estado actual|[10,5]|[0,0]|[7,3]|Sí|
|1|P0|Solicita [8,0]||||Verifica: 8≤7? **NO** → excede necesidad máxima|
|2|Banquero|**Rechaza inmediatamente**|[10,5]|[0,0]|[7,3]|–|

**Caso 3: Solicitud que llevaría a estado inseguro**
Imaginemos que ya se han concedido algunas solicitudes y el estado actual es:
```
asignado[0]=[5,0], necesidad[0]=[2,3]
asignado[1]=[2,1], necesidad[1]=[1,1]
asignado[2]=[2,0], necesidad[2]=[7,2]
disponible = [1,4]  (porque total = [10,5] menos asignado total [9,1] = [1,4])
```
P2 solicita [3,2].

|Paso|Acción|Resultado|
|---|---|---|
|Prueba concesión|`disponible` pasa a [-2,2]|¡No hay suficientes (R0 negativo)! ⇒ denegada por falta de recursos.|

Ahora, un ejemplo donde los recursos alcanzan pero el estado resultante es inseguro.
- `disponible = [2,2]`
- `necesidad`:
    - P0: [1,1]
    - P1: [2,1]
    - P2: [3,2]
P2 pide [1,1]. Si concedemos: `disponible = [1,1]`, `necesidad[2] = [2,1]`.  
Algoritmo de seguridad: ningún proceso tiene necesidad ≤ [1,1] (P0 necesita [1,1] sí, pero P0 terminaría y liberaría sus recursos, luego work = [1,1] + asignado[0] = ... depende del asignado. Si asignado[0]=[2,2], work se convertiría en [3,3] y P1 podría terminar, etc. Hay que ver el caso. Es más didáctico mostrar una corrida con números reales.

Vamos a hacer una corrida en frío **paso a paso** completa para un escenario clásico.
### Corrida en frío completa (3 procesos, 2 recursos)

**Datos iniciales:**
```text
total = [10, 5]
max_nec:
P0: [7,3]
P1: [3,2]
P2: [9,2]
Inicial: asignado = 0, necesidad = max_nec, disponible = [10,5]
```
**Secuencia:**
1. **P0 solicita [5,1]**
    - Verificar: 5≤7, 1≤3, disponibles [10,5] suficientes.
    - Estado tentativo: disponible=[5,4], asignado[0]=[5,1], necesidad[0]=[2,2].
    - Ejecutar algoritmo de seguridad:
        - work = [5,4]
        - P1 necesita [3,2] ≤ [5,4]? Sí → P1 terminaría: work = [5,4]+asignado[1] (0) = [5,4]; finish P1.
        - Luego P0 necesita [2,2] ≤ [5,4]? Sí → termina: work = [5,4]+[5,1]=[10,5]; finish P0.
        - Luego P2 necesita [9,2] ≤ [10,5]? Sí → seguro.
    - **Concede** → estado actual: disponible=[5,4], asignado[0]=[5,1], necesidad[0]=[2,2].
2. **P1 solicita [3,2]**
    - Verificar: 3≤3,2≤2, disponibles [5,4] suficientes.
    - Tentativo: disponible=[2,2], asignado[1]=[3,2], necesidad[1]=[0,0].
    - Seguridad: work=[2,2]; P1 ya tiene necesidad 0, termina, work = [2,2]+[3,2]=[5,4]; luego P0 necesita [2,2] ≤ [5,4] → termina, work=[10,5]; luego P2 → seguro.
    - **Concede** → disponible=[2,2], asignado[1]=[3,2], necesidad[1]=[0,0].
3. **P2 solicita [5,1]**
    - Verificar: 5≤9,1≤2, disponibles [2,2] insuficientes (5>2) → **denegada** (falta recursos).
4. **P2 solicita [2,1]**
    - Verificar: 2≤9,1≤2, disponibles [2,2] suficientes.
    - Tentativo: disponible=[0,1], asignado[2]=[2,1], necesidad[2]=[7,1].
    - Seguridad: work=[0,1]; ningún proceso tiene necesidad ≤ [0,1] (P0 necesita [2,2], P2 [7,1]) excepto P1 que ya tiene necesidad 0. P1 termina: work = [0,1]+[3,2]=[3,3]. Ahora P0 necesita [2,2] ≤ [3,3]? Sí → termina, work=[3,3]+[5,1]=[8,4]. Luego P2 necesita [7,1] ≤ [8,4] → seguro.
    - **Concede** → disponible=[0,1], asignado[2]=[2,1], necesidad[2]=[7,1].
5. **P1 ya terminó su trabajo (necesidad 0)**, entonces en algún momento liberará recursos. Al liberar, `disponible` aumentará.
Este ejemplo muestra cómo el algoritmo evita concesiones que llevarían a un callejón sin salida.
### Problemas y críticas del algoritmo del banquero

|Problema|Descripción|
|---|---|
|**Necesidad de conocer la demanda máxima**|En sistemas reales, los procesos no suelen declarar su necesidad máxima de antemano.|
|**Número fijo de procesos y recursos**|Dificulta la dinámica de creación/terminación de procesos.|
|**Alta sobrecarga computacional**|El algoritmo de seguridad es O(P²·T), puede ser costoso si P y T son grandes.|
|**Los recursos no son intercambiables**|Asume que los recursos de un tipo son idénticos (no diferencias entre impresoras, por ejemplo).|
|**No contempla liberación parcial**|Solo considera liberación total al terminar.|
|**Uso limitado en la práctica**|Raramente implementado en sistemas operativos reales (sí en algunos sistemas de tiempo real o bases de datos).|
### Conclusión
El algoritmo del banquero es un método teórico elegante para evitar deadlocks, pero su aplicación práctica es limitada por los requisitos de información previa y el costo computacional. Sin embargo, entenderlo es fundamental para comprender los conceptos de estado seguro, estrategias de evitación y la relación entre recursos y procesos. El código presentado simula fielmente su comportamiento y puede integrarse con tus librerías `mutex.h` y `semaphore.h`.