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
## El problema de los filósofos comensales
### Descripción
Hay **N filósofos** (normalmente 5) sentados alrededor de una mesa circular. Cada filósofo tiene un plato de espaguetis. Entre cada par de filósofos hay un **tenedor** (o palillo). Para comer, un filósofo necesita **dos tenedores**: el de su izquierda y el de su derecha. Los filósofos alternan entre **pensar** (no usan los tenedores) y **comer** (toman los dos tenedores, comen un rato, luego los sueltan).
### Restricciones
- **Exclusión mutua**: Dos filósofos vecinos no pueden comer simultáneamente porque comparten un tenedor.
- **Sin deadlock**: Que ningún filósofo se quede esperando indefinidamente (bloqueo total).
- **Sin inanición**: Que todos los filósofos puedan comer eventualmente.
- **Concurrencia**: Si no hay conflictos, pueden comer varios filósofos a la vez (por ejemplo, los filósofos pares mientras los impares piensan).
---
## Implementación en C usando tu librería `mutex.h` y `semaphore.h`

La solución clásica usa un **semáforo por tenedor** (inicializado a 1, indicando libre) y un **semáforo adicional** que limita a `N-1` el número de filósofos que pueden intentar tomar tenedores a la vez. Esto evita el deadlock.

```c
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
```
### Explicación de los semáforos

|Semáforo|Valor inicial|Propósito|
|---|---|---|
|`tenedor[i]`|1|Protege el tenedor i (1 = libre, 0 = ocupado)|
|`limite`|N-1|Limita el número de filósofos que pueden estar simultáneamente en la sección de toma de tenedores. Así se evita el deadlock (si N-1 filósofos ya tienen un tenedor, el último no podrá entrar y rompe el ciclo).|
|`print_mutex`|1|Opcional: evita que las impresiones se mezclen (no es esencial para la exclusión mutua de los tenedores).|

---
## Corrida en frío (para N=3, evitando el deadlock)
Tomamos **N=3** (filósofos A(0), B(1), C(2)) y `limite = 2`.  
Estado inicial: todos pensando, tenedores libres (valor=1), `limite=2`.

|Paso|Hilo|Operación|`limite`|Tenedores (0,1,2)|Estado / Comentario|
|---|---|---|---|---|---|
|0|-|Inicio|2|(1,1,1)||
|1|**A**|`wait(&limite)` → OK|1|(1,1,1)|A entra (cupo 1)|
|2|A|`wait(tenedor[0])` → OK|1|(0,1,1)|A toma tenedor izquierdo|
|3|A|`wait(tenedor[1])` → OK|1|(0,0,1)|A toma derecho → **COME**|
|4|**B**|`wait(&limite)` → OK|0|(0,0,1)|B entra (cupo 0)|
|5|B|`wait(tenedor[1])` → tenedor[1]=0 → **BLOQUEADO**|0|(0,0,1)|B espera el tenedor 1 (lo tiene A)|
|6|**C**|`wait(&limite)` → limite=0 → **BLOQUEADO**|0|(0,0,1)|C espera en `limite` (no puede entrar)|
|7|**A**|Termina de comer|0|(0,0,1)||
|8|A|`signal(tenedor[0])`|0|(1,0,1)|Suelta izquierdo|
|9|A|`signal(tenedor[1])`|0|(1,1,1)|Suelta derecho|
|10|A|`signal(&limite)`|1|(1,1,1)|Libera un cupo|
|11|**B**|Se despierta de `wait(tenedor[1])` (ahora vale 1) → continúa|1|(1,0,1)|B toma tenedor[1] (izquierdo)|
|12|B|`wait(tenedor[2])` → tenedor[2]=1 → OK|1|(1,0,0)|B toma derecho → **COME**|
|13|**C**|Sigue bloqueado en `wait(&limite)` porque `limite` está en 1, pero como B ya está comiendo, C todavía no puede entrar|1|(1,0,0)||
|14|B|Termina de comer|1|(1,0,0)||
|15|B|`signal(tenedor[1])`|1|(1,1,0)|Suelta izquierdo|
|16|B|`signal(tenedor[2])`|1|(1,1,1)|Suelta derecho|
|17|B|`signal(&limite)`|2|(1,1,1)|Libera cupo|
|18|**C**|Ahora `wait(&limite)` retorna (pasa a 1)|1|(1,1,1)|C entra|
|19|C|`wait(tenedor[2])` → toma|1|(1,1,0)||
|20|C|`wait(tenedor[0])` → toma|1|(0,1,0)|C come|
|...|...|...|...|...|...|

**Resultado:** No hay deadlock. Los filósofos comen en orden A → B → C (o similar). El semáforo `limite` evita que los tres entren a la vez al protocolo de toma de tenedores, rompiendo la espera circular.

---
## ¿Qué pasaría sin el semáforo `limite`? (Deadlock clásico)
Con N=3 y sin `limite`, si los tres filósofos ejecutan al mismo tiempo:
- A toma tenedor[0] → (0,1,1)
- B toma tenedor[1] → (0,0,1)
- C toma tenedor[2] → (0,0,0)
Luego:
- A intenta tomar tenedor[1] → bloquea (lo tiene B)
- B intenta tomar tenedor[2] → bloquea (lo tiene C)
- C intenta tomar tenedor[0] → bloquea (lo tiene A)
**¡Deadlock!** Todos esperan y nunca liberan.
---
## Problemas del algoritmo de los filósofos comensales

| Problema                   | Descripción                                                             | Consecuencia                                                 |
| -------------------------- | ----------------------------------------------------------------------- | ------------------------------------------------------------ |
| **Deadlock**               | Todos toman un tenedor y esperan el otro                                | Bloqueo total (se soluciona con `limite` u orden asimétrico) |
| **Inanición**              | Un filósofo puede esperar indefinidamente si sus vecinos comen mucho    | Puede ocurrir en ciertas implementaciones no justas          |
| **Condiciones de carrera** | Sin semáforos, dos filósofos podrían tomar el mismo tenedor             | Se evita con semáforos por tenedor                           |
| **Livelock**               | Los filósofos toman y sueltan repetidamente sin progresar               | Raro, pero posible con políticas de “toma si están ambos”    |
| **Sobrecarga**             | Muchos semáforos y cambios de contexto en sistemas con muchos filósofos | Baja eficiencia, pero aceptable para N pequeño               |
### Soluciones adicionales al deadlock
- **Orden asimétrico en la toma de tenedores**: Los filósofos pares toman izquierda→derecha; los impares toman derecha→izquierda. Rompe el ciclo de espera.
- **Monitor con condición**: Solo se permite comer si ambos tenedores están libres; se usa una variable de estado.
- **Semáforo `limite = N-1`** (la que implementamos) – simple y funciona.
---
## Problemas y críticas al algoritmo de los filósofos comensales
Aunque el problema de los filósofos comensales es un clásico para enseñar sincronización, tiene varias limitaciones y críticas tanto en su formulación original como en las soluciones habituales.
### 1. Deadlock (interbloqueo) – el más grave
En la versión ingenua (sin mecanismo de prevención), si cada filósofo toma primero el tenedor izquierdo y luego espera el derecho, puede ocurrir que todos tomen un tenedor y ninguno pueda tomar el segundo. Esto detiene por completo el sistema.

**Crítica:** La solución común (usar un semáforo `limite = N-1` o un orden asimétrico) resuelve el deadlock, pero introduce complejidad adicional y no es obvia para principiantes.
### 2. Inanición (starvation)
Aunque se evite el deadlock, un filósofo puede quedar indefinidamente sin comer si sus vecinos comen con mucha frecuencia. Por ejemplo, en una implementación con prioridades o sin justicia en la adquisición de los semáforos, un filósofo podría ser siempre postergado.

**Crítica:** La solución con `limite = N-1` no garantiza equidad (fairness). Si un filósofo es muy lento para pensar, puede ser saltado. Se necesitarían colas FIFO explícitas o monitores con condiciones más elaboradas.
### 3. Livelock (bloqueo activo)
En algunas variantes (por ejemplo, si un filósofo suelta el tenedor cuando ve que el otro no está disponible y vuelve a intentar), puede ocurrir que todos los filósofos tomen y suelten los tenedores repetidamente sin que nadie coma. Es menos común, pero posible.

**Crítica:** Es difícil de detectar y depurar. Las soluciones con espera activa (`while`) son propensas a este problema.
### 4. Baja escalabilidad
- **Número de semáforos:** Se necesita un semáforo por tenedor, más el semáforo de límite. Para N grande, la sobrecarga de mantenimiento puede ser alta.
- **Contención:** En sistemas con muchos filósofos, la contención sobre los tenedores y el semáforo `limite` crece, reduciendo el paralelismo real.
- **Cuello de botella:** La solución con `limite = N-1` fuerza que solo N-1 filósofos puedan competir, pero eso sigue siendo una limitación.

**Crítica:** El algoritmo no escala bien para cientos o miles de filósofos (hilos). En sistemas reales se prefieren mecanismos como locks distribuidos o algoritmos sin bloqueo.
### 5. Desperdicio de recursos (CPU por espera activa)
Si se implementara con espera activa (en lugar de semáforos bloqueantes), los filósofos consumirían CPU mientras esperan tenedores. Aunque en la solución con semáforos se duermen, la espera sigue implicando cambios de contexto que penalizan el rendimiento.

**Crítica:** La sobrecarga de los semáforos (cada `wait` y `signal` implica llamadas al sistema o al menos operaciones atómicas) puede ser significativa para N grande.
### 6. Dependencia del orden de adquisición de recursos
La solución clásica requiere que **todos los filósofos sigan el mismo orden** al tomar los tenedores (por ejemplo, izquierda luego derecha). Si un filósofo altera el orden, se reintroduce el deadlock. Esto es frágil ante cambios en el código.

**Crítica:** En sistemas reales donde los recursos pueden ser adquiridos en órdenes diferentes, esta solución no es generalizable fácilmente.
### 7. El modelo no refleja muchos problemas reales
- Los filósofos necesitan **dos recursos simultáneamente** (los dos tenedores). En la práctica, muchos procesos necesitan más de un recurso, y la prevención del deadlock es más compleja (por ejemplo, usar orden global de recursos).
- Los filósofos son **simétricos**, pero en sistemas reales los procesos tienen diferentes prioridades o requisitos.
- El tiempo de pensar y comer es aleatorio y no hay restricciones de tiempo real. En sistemas con plazos, este algoritmo no sirve.

**Crítica:** Es un excelente modelo didáctico, pero no debe usarse directamente en software de producción sin adaptaciones.
### 8. Dificultad para probar y depurar
El comportamiento de los filósofos es no determinista. Dependiendo de la intercalación de los hilos, pueden aparecer deadlocks o inanición de forma intermitente. Reproducirlos es difícil, y las soluciones requieren técnicas avanzadas (como monitores o modelos formales).

**Crítica:** Para asegurar la corrección, a menudo se necesita análisis formal o pruebas exhaustivas con herramientas como model checking.
### 9. Soluciones complejas para garantizar equidad
Para evitar la inanición, se pueden usar monitores con una cola de espera ordenada, o asignar turnos mediante un algoritmo de panadería, pero estas soluciones añaden mucha complejidad y overhead.

**Crítica:** El equilibrio entre simplicidad y corrección es delicado. La mayoría de las implementaciones docentes ignoran la inanición y solo evitan el deadlock.
### 10. Problema de la "falsa concurrencia"
Aunque varios filósofos no adyacentes pueden comer al mismo tiempo (por ejemplo, los pares), en muchas soluciones se serializa más de lo necesario debido al semáforo `limite = N-1`. Por ejemplo, si N=5, `limite=4` permite que hasta 4 filósofos intenten tomar tenedores, pero en la práctica los filósofos 0,2 y 4 podrían comer simultáneamente, pero la solución con `limite` no lo impide. El problema es que, aunque no sea un error, la solución puede ser subóptima en paralelismo.

**Crítica:** El algoritmo no maximiza el número de filósofos que pueden comer concurrentemente, sino que previene el deadlock a costa de cierta serialización.

---
## Resumen de críticas en tabla

| Problema                   | Gravedad         | Soluciones conocidas                                   |
| -------------------------- | ---------------- | ------------------------------------------------------ |
| Deadlock                   | Alta             | `limite = N-1`, orden asimétrico, recursos ordenados   |
| Inanición                  | Media            | Colas FIFO en semáforos, monitores con prioridad       |
| Livelock                   | Baja             | Evitar espera activa, usar semáforos bloqueantes       |
| Baja escalabilidad         | Media            | Algoritmos lock-free, locks distribuidos               |
| Sobrecarga de CPU          | Media            | Semáforos (ya lo resuelven), pero hay coste de sistema |
| Dependencia del orden      | Alta             | Diseñar con orden global fijo                          |
| No refleja sistemas reales | Baja (didáctico) | Usar otros modelos (productor-consumidor, etc.)        |
| Dificultad de prueba       | Media            | Herramientas de verificación formal                    |
| Complejidad para equidad   | Media            | Monitores, colas explícitas                            |
| Paralelismo subóptimo      | Baja             | Aceptable para la mayoría de los casos                 |

---
## Conclusión para tu trabajo
El problema de los filósofos comensales es un ejemplo clásico de **sincronización concurrente** que ilustra:
- La necesidad de **exclusión mutua** (los tenedores son recursos compartidos).
- El riesgo de **deadlock** cuando se adquieren recursos en orden.
- El uso de **semáforos** y **técnicas de prevención** (como el límite de `N-1`).

La implementación en C con tu librería `mutex.h` (creación de hilos) y la librería `semaphore.h` (sincronización) es directa y funcional. Puedes ejecutarla para N=5 y observar que no se bloquea. Si eliminas el semáforo `limite`, verás el deadlock (el programa se cuelga). Eso es perfecto para demostrar el problema y la solución.