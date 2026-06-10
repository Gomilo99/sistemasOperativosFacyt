---
alias:
tags:
  - clase
  - estudio
  - sistemas_operativos
creado: 05/06/2026
modificado: 07/06/2026
tipo: Concepto
base:
  - "[[A-Sistemas Operativos]]"
  - "[[04-Recursos]]"
---
## Exclusión Mutua
La **exclusión mutua** es un requisito fundamental en la gestión de procesos concurrentes que garantizan que **solo un proceso a la vez pueda acceder a un recurso compartido** o ejecutar su **sección crítica**.
A continuación, se detallan sus aspectos más importantes:
1. **Sección crítica**: Es el segmento de código dentro de un proceso en el que se manipulan recursos o datos comunes, como variables, tablas o archivos. La característica esencial de un sistema que aplica la exclusión mutua es que, si un proceso está ejecutando su sección crítica, ningún otro programa puede estar ejecutando la suya propia para el mismo recurso.
2. **Progreso**: Si ningún proceso está en su sección crítica y hay algunos que desean entrar, solo aquellos que no estén en su "sección restante" pueden participar en la decisión de quién entra, y esta elección no puede postergarse indefinidamente.
3. **Espera agotada**: Debe existir un límite  en el tiempo o en el número de turnos que un proceso espera para entrar en su sección crítica tras haber realizado la solicitud evitando la inanición.
4. **Velocidad de ejecución**: El mecanismo no debe hacer suposiciones sobre la velocidad relativa de los procesos ni sobre el número de procesadores en el sistema.
La exclusión mutua es necesaria tanto para recursos físicos **intrínsecamente no compartibles**, como una impresora (para evitar que se mezclen las líneas de diferentes trabajos), como para recursos lógicos, como variables de lectura/escritura o bases de datos.

Los mecanismos de exclusión mutua se clasifican en tres categorias:
### 1. Soluciones por Software
Estas soluciones dependen de algoritmos lógicos sin necesidad de soporte especial del hardware (mas allá de la exclusión mutua elemental en el acceso a memoria):
- **Algoritmo de Dekker**: Fue la primera solución correcta para dos procesos. Evolucionó a través de cinco versiones para corregir problemas como la alternancia estricta, el acceso simultáneo y el interbloqueo.
- **Algoritmo de Peterson**: Una solución más simple y elegante que la de Dekker para dos procesos, que utiliza variables indicadoras (banderas) y una variable de turno para resolver conflictos.
- **Algoritmo de la Panadería de Lamport**: Diseñado para $n$ procesos, se basa en un sistema de turnos similar al de las tiendas, donde cada proceso recibe un número y espera su turno.
### 2. Soluciones por Hardware
Aprovechan capacidades específicas del procesador para simplificar la sincronización:
- **Inhabilitación de interrupciones**: En sistemas monoprocesador, desactivar las interrupciones impide que el proceso sea desalojado mientras está en su sección crítica.
- **Instrucciones atómicas**: Instrucciones de máquina especiales que realizan varias acciones (como leer y escribir) como una única unidad interrumpible.
	- **Test-And-Set**: Lee y modifica una variable de forma atómica. Si un proceso la encuentra en falso, la pone en verdadero y entra; los demás deben esperar.
	- **Swap / Exchange**: Intercambia el contenido de dos palabras de memoria de manera atómica.
	- **Compare and Swap (CAS)**: Compara el contenido de una ubicación de memoria con un valor dado y, solo si son iguales, lo modifica por uno nuevo.
### 3. Herramientas de Alto Nivel (Soporte del SO y Lenguajes)
Son mecanismo más abstractos que facilitan el trabajo del programador:
- **Semáforos**: Variables especiales que utilizan operaciones atómicas denominadas  `wait` (o P) y `signal` (o V) para controlar el acceso a recursos. Pueden ser **binarios** (mutex) o **contadores**.
- **Monitores**: Estructuras de lenguajes de programación que encapsulan variables y procedimientos garantizando automáticamente que solo un proceso ejecute un procedimiento dentro del monitor a la vez.
- **Paso de mensajes**: Un método de comunicación y sincronización mediante las primitivas `send` y `receive`, útil tanto en sistemas de memoria compartida como distribuidos.
- **Spinlocks (Cerrojos de bucle sin fin)**: El proceso espera activamente en un bucle hasta que el cerrojo esté disponible. Son útiles en sistemas multiprocesador para secciones de código muy cortas donde el costo de un cambio de contexto sería mayor que la espera.

### Implementaciones en Sistemas Operativos Reales
- **Solaris**: Utiliza **mútex adaptativos**, bloqueos de lectura-escritura y colas de bloqueos (turnstiles).
- **Windows XP**: Emplea **objetos despachadores** como mutantes (mutex en modo kernel o usuario), semáforos, eventos y temporizadores.
- **Linux**: Utiliza principalmente **spinlocks** y **semáforos** para la sincronización del kernel.
## Soluciones de Software
### Algoritmos de Dekker
El algoritmo de Dekker fue la **primera solución de software correcta** al problema de la exclusión mutua para dos procesos concurrentes. Fue desarrollado por el matemático holandés Theodorus Dekker y popularizado por Edsger Dijkstra en la década de 1960.

Su desarrollo se explica tradicionalmente a través de cinco versiones o "intentos" que ilustran los errores comunes en la programación concurrente hasta llegar a la solución definitiva.

Las 5 Versiones del Algoritmo de Dekker

1. Versión 1: Alternancia Estricta

- **Funcionamiento:** Utiliza una variable compartida `turno`. Un proceso solo puede entrar si es su turno; al salir, cede el turno al otro.
- **Problemas:** Obliga a una alternancia estricta, por lo que la velocidad del sistema la dicta el proceso más lento. Si un proceso falla fuera de su sección crítica, bloquea al otro permanentemente.

2. Versión 2: Uso de Banderas (Señales)

- **Funcionamiento:** Cada proceso tiene una bandera (`flag`) para indicar si está en su sección crítica. Antes de entrar, comprueba que la bandera del otro sea `falsa`.
- **Problemas:** **No garantiza la exclusión mutua**. Es posible que ambos procesos comprueben las banderas simultáneamente, las vean en `falso` y entren ambos a la sección crítica antes de poder actualizar las suyas a `verdadero`.

3. Versión 3: Bandera de Intención Previa

- **Funcionamiento:** Para evitar el error de la versión 2, el proceso pone su bandera en `verdadero` **antes** de comprobar la del otro.
- **Problemas:** Provoca un **interbloqueo (deadlock)**. Si ambos ponen su bandera en `verdadero` al mismo tiempo, ambos se quedarán esperando indefinidamente a que el otro baje la suya.

4. Versión 4: Cortesía (Postergación Indefinida)

- **Funcionamiento:** Similar a la versión 3, pero si un proceso ve que el otro también quiere entrar, baja su propia bandera por un momento para permitir el paso del otro y luego lo intenta de nuevo.
- **Problemas:** Puede causar **postergación indefinida (livelock)**. Los procesos podrían entrar en un ciclo infinito de "cortesía" donde ambos bajan y suben sus banderas al mismo ritmo sin que ninguno llegue a entrar nunca.

5. Versión 5: La Solución Correcta

- **Funcionamiento:** Combina las banderas de intención con la variable de `turno` para resolver conflictos de prioridad. Si ambos quieren entrar, el proceso que no tiene el turno baja su bandera y espera a que sea su momento, permitiendo que el favorecido pase.
- **Resultado:** Garantiza la **exclusión mutua**, evita el interbloqueo y asegura que no haya postergación indefinida.

Implementación de la Solución Correcta (Pseudocódigo)

La lógica final para un proceso Pi​ frente a un proceso Pj​ es la siguiente:

```
do {
    flag[i] = TRUE; // El proceso i quiere entrar
    while (flag[j]) { // Si el proceso j también quiere...
        if (turn == j) { // ...y es el turno de j:
            flag[i] = FALSE; // i cede su intención (cortesía)
            while (turn == j); // espera activamente su turno
            flag[i] = TRUE; // reclama su intención de nuevo
        }
    }
    // SECCIÓN CRÍTICA
    turn = j; // Al terminar, cede el turno al otro proceso
    flag[i] = FALSE; // Baja su bandera
    // SECCIÓN RESTANTE
} while (TRUE);
```

Importancia Histórica

Aunque hoy existen soluciones más simples y elegantes como el **Algoritmo de Peterson**, Dekker sentó las bases para resolver la sección crítica exclusivamente mediante software, sin depender de instrucciones especiales de hardware (como _TestAndSet_) ni de la desactivación de interrupciones.
## Algoritmos Clásicos
A continuación se presenta una explicación detallada de los algoritmos clásicos de sincronización, su funcionamiento, implementación y contexto histórico.
### 1. Problema del Productor-Consumidor (Buffer Acotado y No Acotado)
- **Planteamiento e Historia**: Este modelo, también conocido como el problema del buffer limitado, es un paradigma representativo de muchos procesos que cooperan en un sistema operativo. Fue desarrollado para ilustrar el poder de las primitivas de sincronización. Plantea un escenario con uno o más **productores** que generan datos y un **consumidor** que los retira de un almacén compartido (buffer).
- **Qué resuelve**: Garantiza que el consumidor no intente retirar datos de un buffer vacío y que el productor no añada datos a un buffer lleno (en el caso acotado). Además, asegura la **exclusión mutua** para que dos procesos no accedan simultáneamente al buffer, evitando la corrupción de datos.
- **Requerimientos**: Semáforos (uno binario para exclusión mutua y dos contadores para rastrear vacíos y llenos) o monitores.
**Pseudocódigo (Buffer Acotado con Semáforos)**
```pseudocodigo
// Datos compartidos
semaphore mutex = 1;      // Controla el acceso al buffer
semaphore vacio = n;      // Cuenta espacios libres (n = tamaño del buffer)
semaphore lleno = 0;      // Cuenta elementos producidos

// Proceso Productor
void productor() {
    while (true) {
        item = producir_item(); 
        wait(vacio);       // Espera si el buffer está lleno
        wait(mutex);       // Entra a la sección crítica
        añadir_al_buffer(item);
        signal(mutex);     // Sale de la sección crítica
        signal(lleno);     // Incrementa la cuenta de elementos listos
    }
}

// Proceso Consumidor
void consumidor() {
    while (true) {
        wait(lleno);       // Espera si el buffer está vacío
        wait(mutex);       // Entra a la sección crítica
        item = quitar_del_buffer();
        signal(mutex);     // Sale de la sección crítica
        signal(vacio);     // Incrementa la cuenta de espacios libres
        consumir_item(item);
    }
}
```
- **Corrida en frío (Buffer vacío)**: 
	1. El `Consumidor` intenta entrar, ejecuta `wait(lleno)` y se bloquea (valor 0).
	2. El `Productor` produce un item, ejecuta `wait(vacio)` (valor n-1), `wait(mutex)` y añade un item.
	3. Al terminar, ejecuta `signal(lleno)`, lo que despierta al `Consumidor`.

---
### 2. Problema de los Lectores y Escritores
- **Planteamiento e Historia**: Se centra en una base de datos o área de datos compartida entre múltiples procesos concurrentes. Desde su formulación, se ha usado para probar casi cada nueva primitiva de sincronización inventada.
- **Qué resuelve**: Permite que cualquier número de **lectores** acceda al recurso (ya que leer no altera los datos), pero garantiza que los **escritores** tengan acceso exclusivo (nadie más puede leer ni escribir mientras un escritor actúa). Resuelva la posible inconsistencia de datos y mejora la eficiencia frente a soluciones de exclusión mutua general.
- **Requerimientos**: Semáforos para proteger la variable de conteo de lectores y para bloquear el recurso contra escritores.
**Pseudocódigo (Prioridad a Lectores)**
```
// Datos compartidos
int cont_lect = 0;        // Número de lectores activos
semaphore mutex_lect = 1; // Protege la variable cont_lect
semaphore db = 1;         // Controla el acceso a la base de datos

// Proceso Lector
void lector() {
    wait(mutex_lect);
    cont_lect++;
    if (cont_lect == 1) wait(db); // El primer lector bloquea a los escritores
    signal(mutex_lect);
    
    leer_datos();                 // SECCIÓN CRÍTICA (Lectura)
    
    wait(mutex_lect);
    cont_lect--;
    if (cont_lect == 0) signal(db); // El último lector libera la DB
    signal(mutex_lect);
}

// Proceso Escritor
void escritor() {
    wait(db);                     // Espera acceso exclusivo
    escribir_datos();             // SECCIÓN CRÍTICA (Escritura)
    signal(db);
}
```
- **Corrida en frío**:
	1. Un `Lector1` llega, bloquea `db` y lee.
	2. Llega un `Escritor` y se bloquea en `wait(db)`.
	3. Llega un `Lector2`, incrementa `cont_lect` a 2 y lee directamente sin esperar en `db`.
	4. `Lector1` y `Lector2` terminan; el ultimo libera `db` permitiendo que el `Escritor` entre.
---
### 3. Problema del Barbero Dormilón
- **Planteamiento e Historia**: Propuesto originalmente por Dijkstra como una analogía de la coordinación entre procesos y recursos. Describe una peluquería con un barbero, una silla de trabajo y una sala de espera con sillas limitadas.
- **Qué resuelve**: Coordina la interacción entre un servidor y clientes sin caer en competencia. Evita que el barbero "corte el aire" si no hay nadie sentado y gestiona el rechazo de clientes si la capacidad está llena.
- **Requerimientos**: Semáforos para señalizar clientes listos, barberos libres y exclusión mutua para el conteo de sillas.
**Pseudocódigo simplificado**
```
// Datos compartidos
semaphore clientes = 0;   // Clientes esperando
semaphore barbero = 0;    // Barberos listos
semaphore mutex = 1;      // Exclusión mutua para sillas
int sillas_libres = n;    // Número de sillas en la sala

// Proceso Barbero
void barbero() {
    while (true) {
        wait(clientes);    // Duerme si no hay clientes
        wait(mutex);
        sillas_libres++;   // Libera una silla de espera
        signal(barbero);   // El barbero está listo
        signal(mutex);
        cortar_pelo();     // Sección crítica
    }
}

// Proceso Cliente
void cliente() {
    wait(mutex);
    if (sillas_libres > 0) {
        sillas_libres--;   // Se sienta en la sala
        signal(clientes);  // Despierta al barbero
        signal(mutex);
        wait(barbero);     // Espera a que el barbero lo llame
        recibir_corte();
    } else {
        signal(mutex);     // Se va porque no hay sillas
    }
}
```
- **Corrida en frío**:
	1. El `Barbero` se bloquea en `wait(clientes)`.
	2. Llega un `Cliente`, decremente `sillas_libres`, ejecuta `signal(clientes)` despertando al `Barbero` y espera en `wait(barbero)` despierta, indica que está listo (`signal(barbero`) y corta el pelo.
---
### 4. Problema de los Filósofos Comensales

- **Planteamiento e Historia:** Propuesto por Dijkstra, es un problema clásico para ilustrar los desafíos de la sincronización, el **interbloqueo** (_deadlock_) y la **inanición** (_starvation_). Cinco filósofos pasan la vida pensando y comiendo espaguetis en una mesa circular con cinco tenedores (uno entre cada plato).
- **Qué resuelve:** Representa la necesidad de asignar recursos escasos entre procesos de manera que se evite que todos queden bloqueados esperando un recurso que otro tiene (espera circular).
- **Requerimientos:** Una estructura que controle el estado de cada filósofo (`pensando`, `hambriento`, `comiendo`) y semáforos para los tenedores o una solución con monitor.

**Pseudocódigo (Solución con Monitor para evitar Deadlock)**
```
monitor CenaFilosofos {
    enum {PENSANDO, HAMBRIENTO, COMIENDO} estado[35];
    condition auto[35];

    void tomar_tenedores(int i) {
        estado[i] = HAMBRIENTO;
        test(i); // Intenta comer si los vecinos no comen
        if (estado[i] != COMIENDO) auto[i].wait();
    }

    void dejar_tenedores(int i) {
        estado[i] = PENSANDO;
        test((i + 4) % 5); // Avisa al vecino izquierdo
        test((i + 1) % 5); // Avisa al vecino derecho
    }

    void test(int i) {
        if ((estado[(i + 4) % 5] != COMIENDO) && (estado[i] == HAMBRIENTO) &&
            (estado[(i + 1) % 5] != COMIENDO)) {
            estado[i] = COMIENDO;
            auto[i].signal();
        }
    }
}
```

- **Corrida en frío:** 
	1. El Filósofo 1 tiene hambre, llama a `test(1)`, comprueba que sus vecinos (0 y 2) no comen y empieza a comer. 
	2. El Filósofo 2 tiene hambre, llama a `test(2)`, pero como el vecino 1 está comiendo, se bloquea en `auto.wait()`. 
	3. Cuando el Filósofo 1 termina, llama a `test(2)`, lo que despierta al Filósofo 2.
### Ejemplos de la realidad

| Modelo                    | Ejemplo                                                                                                                                                                                   |
| ------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Productor-Consumidor**  | Un **compilador** generando código ensamblador que es consumido por un **ensamblador**. Tambien el manejo de una cola de impresión.                                                       |
| **Lectores y Escritores** | El **catalogo de una biblioteca** en línea donde miles de usuarios consultan (leen) y solo los bibliotecarios actualizan (escriben). Sistemas de reservación de aerolíneas.               |
| **Barbero Dormilón**      | Un **ayudante de cátedra** en su oficina que ayuda a estudiantes; si no hay nadie, duerme, y si hay muchos, estos esperan en sillas en el pasillo.                                        |
| **Filósofos Comensales**  | Procesos que compiten por el **acceso exclusivo a un número limitado de dispositivos** de E/S, como unidades de cinta o escáneres, donde se necesita más de uno para completar una tarea. |
