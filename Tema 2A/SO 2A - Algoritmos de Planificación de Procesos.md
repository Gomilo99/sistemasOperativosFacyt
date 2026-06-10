---
alias:
tags:
  - estudio
  - sistemas_operativos
creado: 28/05/2026
modificado: 29/05/2026
tipo: Concepto
base:
  - "[[A-Sistemas Operativos]]"
  - "[[04-Recursos]]"
---
## Algoritmos básicos
### FCFS (First-Come, First-Served)
#### Funcionamiento

- **No apropiativo (no preemptive).**
- Atiende procesos en el **orden de llegada** a la cola de listos.
- Simple: cuando un proceso toma CPU, **no la suelta** hasta terminar o bloquearse (E/S).
### Round Robin
#### Funcionamiento
- **Apropiativo**, orientado a **time-sharing**.
- Cola FIFO circular: cada proceso recibe un **quantum q**.
- Si no termina, vuelve al final de la cola.
#### Comentarios clave
- Si q es muy pequeño ⇒ muchos cambios de contexto.
- Si q es muy grande ⇒ se parece a FCFS.
### Selfish Round Robin (SRR)
#### Funcionamiento (idea)
- Variante de RR que intenta ser “más justa”/eficiente:
    - Mantiene al menos dos conjuntos/colas: **activos** y **nuevos/espera**.
    - Se asigna una “**prioridad dinámica / crédito**” que cambia con el tiempo.
    - Los procesos que “se portan bien” (p. ej., interactivos) pueden obtener mejor acceso (dependiendo de la formulación concreta del SRR que te pidan).
### Planificación por Prioridades
#### Funcionamiento
- Puede ser **no apropiativa o apropiativa**.
- La CPU se asigna al proceso con **mayor prioridad** (según convención).
- Problema típico: **inanición** de baja prioridad.
- Solución típica: **aging** (envejecimiento) aumenta prioridad con el tiempo en espera.
### Colas Multinivel (MLQ - Multilevel Queue)
#### Funcionamiento
- Varias colas **separadas por clase** (ej.: interactivos, batch, sistema).
- Cada cola puede tener su propio algoritmo (RR para interactivos, FCFS para batch).
- Regla entre colas:
    - **Prioridad fija** (cola 0 siempre gana), o
    - “**Time slicing entre colas**” (porcentaje de CPU para cada cola).
### Colas Multinivel con Retroalimentación (MLFQ)
#### Funcionamiento
- Similar a MLQ, pero **los procesos pueden cambiar de cola**.
- Idea central:
    - Procesos nuevos empiezan en cola alta (alta prioridad, quantum pequeño).
    - Si usan mucho CPU ⇒ bajan de cola (se consideran “CPU-bound”).
    - Si se bloquean por E/S ⇒ suelen quedarse arriba (se consideran “I/O-bound”/interactivos).
- Evita, en parte, conocer ráfagas a priori.
## Algoritmos Complejos
### Sistemas de Tiempo Real (Real-Time Scheduling)
#### Funcionamiento
Se distinguen:
- **Hard Real-Time:** perder un deadline es inaceptable.
- **Soft Real-Time:** perder un deadline degrada calidad, pero no es “fatal”.

Algoritmos clásicos:
- **EDF (Earliest Deadline First)**: ejecuta el que tiene deadline más próximo (óptimo en CPU única bajo ciertas condiciones).
- **RMS (Rate Monotonic Scheduling)**: prioridad fija inversa al período (tareas más frecuentes ⇒ más prioridad).
### Planificación para Linux
#### Funcionamiento (visión moderna)
- Linux usa clases de planificación:
    - **SCHED_FIFO / SCHED_RR** (tiempo real POSIX),
    - **SCHED_DEADLINE** (basado en EDF con parámetros runtime/deadline/period),
    - y para “normal” (no RT) históricamente: **CFS (Completely Fair Scheduler)**.
- CFS modela “equidad” usando **tiempo virtual** y estructura tipo **árbol balanceado** (red-black tree) para escoger el que “menos CPU ha recibido” relativo a su peso (nice).
### Planificación para Windows
Windows planifica **hilos (threads)**, no "procesos" directamente. Un proceso es un contenedor; lo que compite por la CPU son sus hilos.
- El planificador es **apropiativo**, basado en prioridades, y con **time slicing** (quantum) para repartir CPU entre hilos de la misma prioridad.
#### Funcionamiento
- **Unidad planificada**: thread (hilos)
- **Apropiativo**: Windows puede interrumpir el hilo actual si:
	- Queda listo un hilo de **mayor prioridad** o,
	- el hilo actual **consume su quantum**, o
	- El hilo se **bloquea** (E/S, espera de lock, evento, etc.), o
	- Se produce un evento del sistema (p,ej. afinidad, migración, etc.).
##### Prioridades
- Hay prioridad **base** y una **prioridad dinámica efectiva**.
- Regla principal: **siempre corre el hilo listo con mayor prioridad**.
- Si hay varios hilos con la **misma prioridad**, se aplica un comportamiento tipo **Round Robin** (turnos) con quantum.
##### Prioridad Dinámica y "Boosts"
Para mejorar la *responsividad* (especialmente en escritorio):
- Windows aplica **boosts temporales** (aumentos de prioridad) a hilos que:
	- acaban de salir de E/S (I/O completion),
	- interactúan con el usuario (GUI),
	- o se consideran "interactivos".
- Luego, la prioridad suele **decaer gradualmente** hacia la base.
**Resultado**: los hilos interactivos / I/O-bound tienden a sentirse "rápidos", y los CPU-bound tienden a quedarse en su prioridad base.
##### Quantum (time slice)
Un hilo que se ejecuta consume su quantum. Cuando se agota:
- Si hay otros hilos listos de la **misma prioridad**, pasa al final de la cola (RR)
- Si no hay, puede seguir (dependiendo del estado en la cola de listos).
El quantum puede variar según:
- Políticas de sistema (ej, "Programs vs Backgrounds services").
- Versión / edición.
- Tipo de carga.
##### Multiprocesador / Multicore
En sistemas con múltiples CPUs/cores:
- Windows tiene colas "ready" por prioridad y mecanismos para escoger en qué core corre un hilo.
- Considera **afinidad** (mantener un hilo en el mismo core si conviene por caché).
- Hace **balanceo** cuando hay cores ociosos y otros saturados.
#### Ventajas
1. **Muy buena respuesta interactiva**: los boosts priorizan la sensación de fluidez (mouse/teclado/UI).
2. **Expropiativo por prioridad**: un hilo crítico puede “entrar” rápido.
3. **Round Robin dentro del mismo nivel**: evita que un hilo monopolice si hay competidores iguales.
4. **Soporte práctico para cargas mixtas**: servidor + escritorio + I/O intensivo.
5. **Afinidad y balanceo** (en multicore): mejora throughput y localidad de caché.
#### Desventajas
1. **No es determinista**: por boosts dinámicos, es difícil predecir exactamente el orden (malo para hard real-time).
2. **Posible inanición** (starvation) en casos extremos: hilos de baja prioridad pueden sufrir si hay alta carga de prioridades superiores.
3. **Overhead por cambios de contexto**: al ser apropiativo y con time slicing, puede aumentar el número de context switches.
4. **Complejidad de análisis**: es más difícil “modelar” su comportamiento que FCFS/SJF/etc.
#### Eficiencia (tiempo de ejecución) y rendimiento esperado
##### Overhead del planificador
- **Medio/alto** comparado con FCFS:
    - necesita manejar colas por prioridad,
    - recalcular prioridades dinámicas,
    - aplicar boosts/decay,
    - y decidir migraciones/afinidad en multicore.
- En la práctica, está muy optimizado y el overhead suele ser aceptable para uso general.
##### Eficiencia para diferentes objetivos
- **Tiempo de respuesta (interactive latency):** generalmente **muy bueno**.
- **Throughput total (trabajo por segundo):** bueno, especialmente en multicore, pero puede verse afectado por exceso de preemption si hay demasiados hilos compitiendo.
- **Fairness (equidad pura):** no es “perfectamente justo” porque prioriza UX y heurísticas (boosts).
#### Recursos necesarios para implementarlo
##### Estructuras de datos (software)
1. **Colas/listas de listos por prioridad** (ready queues), típicamente:
    - un conjunto de colas para cada nivel de prioridad,
    - con acceso rápido para encontrar la prioridad más alta con hilos listos.
2. **Bitmaps / tablas** para localizar rápidamente la cola de prioridad más alta no vacía.
3. **Estructuras por hilo** (TCB): prioridad base/dinámica, estado, afinidad, contadores de quantum, etc.
4. **Contabilidad y temporizadores** para:
    - controlar el consumo de quantum,
    - aging/decay/boost.
##### Soporte de hardware (típico)
1. **Timer/clock interrupt** de alta resolución (para quantum y preemption).
2. **Soporte de interrupciones** para E/S (disparan wakeups y boosts).
3. MMU/soporte de cambio de contexto (registros, TLB, etc.) — común a cualquier SO moderno.
4. En multicore: mecanismos de **IPI** (inter-processor interrupts) para coordinación/balanceo.

### Planificación en sistemas Multiprocesadores (Multi-CPU / SMP)
#### Funcionamiento
Problemas clave:
- **Balanceo de carga** (load balancing): repartir trabajo entre CPUs.
- **Afinidad de CPU**: intentar que un proceso/hilo corra en la misma CPU para aprovechar caché (cache locality).
- En SMP, puede haber:
    - **cola global** de listos (una sola),
    - o **colas por CPU** (más escalable) con “work stealing”.
### Planificación en sistemas Multicore
#### Funcionamiento
Similar a multiprocesador, pero con efectos extra:
- Cachés compartidas (L3), **SMT/Hyper-Threading**, interferencias.
- Planificadores modernos consideran topología: **NUMA**, “core siblings”, etc.
- Objetivo: rendimiento + eficiencia energética + afinidad de caché.
### Planificación de hilos (Thread Scheduling)
#### Funcionamiento

- Hoy casi siempre el planificador del kernel planifica **hilos**, no “procesos” completos.
- Decisiones:
    - qué hilo ejecutar,
    - quantum,
    - prioridades,
    - sincronización (bloqueos), inversión de prioridad, etc.
- En user-level threads: el planificador puede estar en biblioteca (M:N, N:1).
### Green threads y Goroutines (Go) + cómo se planifican
#### Green threads
- Hilos **en espacio de usuario** (no 1:1 con hilos del kernel).
- El “runtime” o biblioteca hace el **scheduling**.
- Ventajas: creación/cambio más barato.
- Desventaja histórica: si un green thread hace una syscall bloqueante (sin mecanismos), puede bloquear a todos (dependiendo del modelo).
#### Goroutines (Go)
- Son “green threads” modernos gestionados por el **runtime de Go**.
- Go usa un scheduler tipo **M:P:G**:
    - **G** = goroutine,
    - **M** = thread del sistema (kernel thread),
    - **P** = “processor” lógico del runtime que ejecuta goroutines (cantidad ≈ `GOMAXPROCS`).
- Usa colas locales + **work stealing**; intenta mantener muchas goroutines multiplexadas sobre menos threads.
- El runtime intenta evitar bloqueos globales: integra syscalls, timers, network poller, etc.