---
creado: 26/05/2026
modificado: 26/05/2026
---
Para entender el esquema de 7 estados, primero debemos establecer qué es un proceso y algunos conceptos clave que el Sistema Operativo (SO) utiliza para administrarlos.

### Introducción a los Procesos
Desde la perspectiva del sistema operativo, un **proceso es un programa en ejecución**. Mientras que un programa es una entidad pasiva (un archivo con código guardado en el disco), un proceso es una entidad activa. 

Un proceso no solo incluye el código del programa, sino también:
*   **El Contador de Programa (Program Counter):** Que indica la próxima instrucción a ejecutar.
*   **La Pila (Stack):** Que contiene datos temporales como parámetros de funciones, direcciones de retorno y variables locales.
*   **La Sección de Datos:** Que contiene variables globales.
*   **El Montículo (Heap):** Memoria que se asigna dinámicamente durante el tiempo de ejecución.

### Temas Auxiliares Clave
Para gestionar los procesos y sus cambios de estado, el SO se apoya en los siguientes conceptos:
1.  **Bloque de Control de Proceso (PCB - Process Control Block):** Es una estructura de datos que el SO utiliza para almacenar toda la información sobre un proceso específico (su estado actual, ID, registros de CPU, información de memoria, etc.). Es esencialmente el "DNI" del proceso.
2.  **Cambio de Contexto (Context Switch):** Cuando el procesador cambia de un proceso a otro, el SO debe guardar el estado (contexto) del proceso actual en su PCB y cargar el contexto guardado del nuevo proceso.
3.  **Planificador (Scheduler):** Es el componente del SO encargado de decidir qué proceso en estado "Listo" debe pasar a ejecutarse en la CPU y por cuánto tiempo.
4.  **Intercambio (Swapping):** Es la técnica de mover procesos completos de la memoria principal (RAM) a la memoria secundaria (disco duro/SSD) y viceversa. **Este concepto es la razón de ser del modelo de 7 estados**, ya que permite liberar espacio en la RAM cuando esta se llena.

---

### El Esquema de 7 Estados de Procesos
El modelo clásico de gestión de procesos tiene 5 estados (Nuevo, Listo, En Ejecución, Bloqueado y Terminado). Sin embargo, la memoria principal es limitada. ¿Qué pasa si todos los procesos en memoria se bloquean esperando entrada/salida (E/S) y la CPU se queda ociosa? 

Para resolver esto, el SO realiza *swapping*: mueve procesos bloqueados o listos al disco para liberar RAM. Esto introduce dos nuevos estados ("Suspendidos"), creando el modelo de 7 estados:

#### 1. Nuevo (New)
El proceso se acaba de crear, pero el SO aún no lo ha admitido en el grupo de procesos ejecutables (típicamente, su código y datos aún no se han cargado en la memoria principal).

#### 2. Listo (Ready)
El proceso está cargado en la memoria principal y tiene todos los recursos que necesita para ejecutarse, excepto la CPU. Está en una cola esperando que el planificador del SO le asigne tiempo de procesador.

#### 3. En Ejecución (Running)
El proceso tiene el control de la CPU y sus instrucciones se están ejecutando en este momento.

#### 4. Bloqueado / En Espera (Blocked / Waiting)
El proceso no puede continuar ejecutándose porque está esperando que ocurra un evento externo. Por lo general, esto es una operación de Entrada/Salida (esperar a que el usuario presione una tecla, leer un archivo del disco, esperar un paquete de red, etc.). Mientras está aquí, el proceso suelta la CPU para que otros puedan usarla.

#### 5. Terminado (Terminated / Exit)
El proceso ha terminado su ejecución (ya sea con éxito o por un error) y el SO está liberando sus recursos (memoria, archivos abiertos) y eliminando su PCB.

#### 6. Bloqueado y Suspendido (Blocked Suspended)
*Aquí entra el concepto de swapping.* Si la memoria RAM se llena, el SO puede decidir tomar un proceso que está en estado "Bloqueado" y moverlo al disco duro. El proceso sigue esperando el evento (como una lectura de disco), pero ya no ocupa espacio valioso en la RAM.

#### 7. Listo y Suspendido (Ready Suspended)
Existen dos formas de llegar a este estado:
*   Un proceso en "Bloqueado y Suspendido" recibe el evento que estaba esperando (por ejemplo, el usuario presionó la tecla). Ya no está bloqueado, está "Listo", pero sigue guardado en el disco duro porque no hay RAM disponible.
*   El SO, por políticas de prioridad o gestión de carga, decide quitar un proceso "Listo" de la RAM y mandarlo al disco duro para hacerle espacio a un proceso más prioritario.

Cuando se libera suficiente memoria RAM, un proceso "Listo y Suspendido" es traído de vuelta a la memoria principal, pasando al estado "Listo" normal para competir nuevamente por la CPU.







Profundizar en la teoría de Sistemas Operativos (SO) revela que el esquema de 7 estados no es un capricho teórico, sino una solución brillante a un problema físico de hardware: la diferencia de velocidad entre la CPU y los dispositivos de Entrada/Salida (E/S), y la limitación de la memoria RAM.

Aquí tienes un desglose profundo de los temas auxiliares, el origen del modelo, una corrida en frío (dry run) y sus limitaciones/optimizaciones.

---

### 1. Profundidad en los Temas Auxiliares Clave

Para que los 7 estados funcionen, el SO opera una maquinaria compleja "detrás de escena":

*   **PCB (Bloque de Control de Proceso):** No es solo una etiqueta; es una estructura en la memoria del *Kernel*. Contiene:
    *   *Estado actual* (Listo, Bloqueado, etc.).
    *   *Registros del procesador:* Cuando el proceso se pausa, todos los valores matemáticos que la CPU estaba usando se guardan aquí.
    *   *Límites de memoria:* Punteros a dónde empieza y termina el proceso en la RAM, para evitar que un proceso lea datos de otro (seguridad).
    *   *Archivos abiertos:* Lista de descriptores de archivos que el proceso está usando.
*   **El Cambio de Contexto (Context Switch):** Es una operación de **pura sobrecarga (overhead)**. Mientras ocurre un cambio de contexto, el sistema no hace ningún trabajo útil (ningún programa avanza). El SO debe: pausar el proceso A, guardar su estado en su PCB, buscar el PCB del proceso B, restaurar el estado en la CPU y saltar a la instrucción de B. Los SO modernos intentan minimizar esto porque consume valiosos microsegundos.
*   **Los 3 Tipos de Planificadores (Schedulers):** Esto es crucial para los 7 estados.
    1.  *A corto plazo (CPU Scheduler):* Mueve procesos de "Listo" a "En Ejecución". Es rapidísimo y actúa cada pocos milisegundos.
    2.  *A mediano plazo (Swapper/Intercambiador):* **El rey del esquema de 7 estados**. Se encarga de sacar procesos de la RAM al disco (Suspendidos) y traerlos de vuelta para controlar el grado de multiprogramación (cuántos procesos hay en RAM).
    3.  *A largo plazo (Admission Scheduler):* Decide qué programas nuevos (estado "Nuevo") entran a la cola de "Listos".
*   **Memoria de Intercambio (Swap / Pagefile):** Es una partición o archivo especial en el disco duro/SSD que el SO trata como si fuera una "extensión" lenta de la memoria RAM.

---

### 2. El Origen del Esquema de 7 Estados

**El problema original:** Los primeros sistemas tenían un modelo de 5 estados (Nuevo, Listo, Ejecutando, Bloqueado, Terminado). Sin embargo, las operaciones de E/S (como leer un disco mecánico) son *millones* de veces más lentas que los ciclos de la CPU. 
En un sistema ocupado, todos los procesos en RAM podían terminar esperando E/S al mismo tiempo (todos en estado "Bloqueado"). Como resultado, **la CPU se quedaba ociosa (idle)**, desperdiciando capacidad de cómputo, y no se podían cargar procesos nuevos porque la RAM estaba llena de procesos bloqueados.

**La solución:** Añadir el concepto de **Suspensión**. El SO toma a los procesos bloqueados, los copia enteramente al disco duro, libera esa RAM y carga nuevos procesos para mantener a la CPU ocupada. Esto obligó a dividir los estados para saber quién está en RAM y quién en disco duro, naciendo los estados "Bloqueado y Suspendido" y "Listo y Suspendido".

---

### 3. Ejemplo Práctico: Corrida en Frío (Dry Run)

Imagina un proceso `P1` que es un software de edición de video exportando un proyecto muy pesado, en una computadora con poca RAM (ej. 4GB).

1.  **Nuevo:** Haces doble clic en el editor de video. El SO crea el PCB para `P1`.
2.  **Listo:** El código base de `P1` se carga en la RAM. Entra a la cola esperando su turno.
3.  **En Ejecución:** El planificador a corto plazo le da la CPU a `P1`. Empieza a calcular los píxeles del video.
4.  **Bloqueado:** `P1` necesita leer un archivo de audio gigante del disco duro. Hace una petición al disco y suelta la CPU. El SO pasa a `P1` a Bloqueado.
5.  **Bloqueado y Suspendido:** Mientras el disco duro (que es lento) busca el audio, tú abres Google Chrome (que consume mucha RAM). La RAM se llena. El planificador a mediano plazo decide quitar a `P1` de la RAM y mandarlo al archivo de paginación (Swap) en el disco.
6.  **Listo y Suspendido:** El disco duro finalmente encuentra el archivo de audio. El evento que `P1` esperaba ya ocurrió. `P1` ya no está bloqueado, está "Listo", **pero** sigue atrapado en el disco (Swap) porque Chrome sigue ocupando toda la RAM.
7.  **Listo:** Cierras algunas pestañas de Chrome. Se libera RAM. El SO mueve a `P1` desde el disco de vuelta a la memoria RAM.
8.  **En Ejecución:** `P1` vuelve a tomar la CPU y continúa exportando el video.
9.  **Terminado:** El video se exporta con éxito. El SO destruye el PCB y libera todos los recursos.

---

### 4. Limitaciones, Problemas y Optimizaciones modernas

#### Limitaciones y Problemas
*   **El temido "Thrashing" (Hiperpaginación):** Es el peor escenario del esquema de 7 estados. Ocurre cuando la RAM es tan pequeña que el SO pasa más tiempo moviendo procesos entre la RAM y el disco duro (Swapping) que ejecutándolos en la CPU. El sistema entero se "congela" y el disco duro se pone al 100%.
*   **Latencia extrema:** Si un proceso "Listo y Suspendido" necesita ejecutarse de urgencia, primero debe ser copiado del disco a la RAM. Si el disco es un HDD antiguo, el proceso sufrirá un retraso masivo.

#### Optimizaciones en Sistemas Operativos Modernos (Windows 11, Linux, macOS)
Hoy en día, los SO no siempre mueven procesos *enteros* como sugiere la teoría clásica, sino que usan técnicas más granulares:
1.  **Paginación bajo demanda (Demand Paging):** En lugar de mandar todo el proceso al disco, el SO divide el proceso en "páginas" de 4 Kilobytes. Solo manda al disco las páginas inactivas (por ejemplo, el menú de ayuda del programa que no estás usando), dejando el núcleo del programa en RAM.
2.  **Compresión de Memoria (ZRAM / Memory Compression):** Antes de enviar un proceso al lento disco duro, Windows y macOS intentan *comprimir* los datos en la misma RAM (como si fuera un archivo .zip). Descomprimir en la CPU es mucho más rápido que leer del disco duro.
3.  **SSDs NVMe:** La velocidad extrema de los discos de estado sólido modernos hace que las transiciones hacia y desde los estados "Suspendidos" sean casi imperceptibles para el usuario, enmascarando los cuellos de botella del swapping clásico.

**Tip práctico:** Si abres el Administrador de Tareas en Windows o usas `htop` en Linux, puedes ver esto en acción. En Linux, verás procesos con estado `S` (Sleeping/Bloqueado), `R` (Running), o `D` (Disk Sleep / Bloqueo ininterrumpible por E/S). En Windows, los procesos en estados "Suspendidos" a menudo aparecen con un ícono de una hojita verde o bajo el rubro de "Memoria Comprimida".










### 1. ¿Qué es la Sobrecarga (Overhead)?

En el contexto de los sistemas operativos, la **sobrecarga (o *overhead*)** es el tiempo de procesamiento, memoria u otros recursos que el sistema gasta haciendo "trabajo administrativo" en lugar de ejecutar el código útil de los programas del usuario.

Piensa en un taller mecánico: el tiempo que el mecánico pasa arreglando el motor es el "trabajo útil". El tiempo que pasa limpiando sus herramientas, guardándolas y sacando unas nuevas para el siguiente auto es la **sobrecarga**. Es un trabajo necesario para que el taller funcione bien, pero durante ese tiempo no se está reparando ningún auto.

**El mayor ejemplo de sobrecarga es el Cambio de Contexto (Context Switch):**
Como vimos antes, para cambiar del Proceso A al Proceso B, el SO debe:
1. Pausar A.
2. Guardar todos los valores de A en su PCB (memoria).
3. Buscar a B en la cola de Listos.
4. Cargar los valores del PCB de B en el procesador.
5. Iniciar B.

Todo este proceso toma tiempo (microsegundos o milisegundos). Durante ese breve instante, **la CPU trabaja al 100%, pero el rendimiento útil para el usuario es 0%**. Si un SO cambia de procesos con demasiada frecuencia, la sobrecarga será tan alta que la computadora se volverá lenta, dedicando más tiempo a administrar los cambios que a ejecutar las aplicaciones.

---

### 2. Algoritmos No Apropiativos (Non-Preemptive / Cooperativos)

La palabra clave aquí es **Voluntario**. 

En un algoritmo no apropiativo, una vez que el sistema operativo le entrega la CPU a un proceso, **el proceso mantiene el control total de la CPU hasta que él mismo decida soltarla**. El SO no se la puede arrebatar.

El proceso solo soltará la CPU si:
1.  Termina su ejecución por completo.
2.  Necesita hacer una operación de Entrada/Salida (leer el disco, esperar a la red) y pasa voluntariamente al estado "Bloqueado".

*   **Ventajas:** Tienen muy **baja sobrecarga (overhead)** porque hay muchos menos cambios de contexto. Son fáciles de programar y no sufren problemas complejos de sincronización (condiciones de carrera), ya que nadie interrumpe a un proceso mientras modifica datos.
*   **Desventajas:** Un proceso "egoísta" o mal programado (por ejemplo, con un bucle infinito) acaparará la CPU para siempre, "congelando" todo el sistema. 
*   **Ejemplos:** Algoritmo FCFS (First-Come, First-Served / El primero en llegar es el primero en servirse) o SJF (Shortest Job First no apropiativo).
*   **Contexto histórico:** Así funcionaban Windows 3.1 y el Mac OS clásico. Si un programa fallaba, tenías que reiniciar toda la computadora porque el SO no podía recuperar el control.

---

### 3. Algoritmos Apropiativos (Preemptive)

La palabra clave aquí es **Interrupción (o Arrebato)**. ("Apropiativo" viene de que el SO se "apropia" de vuelta de la CPU).

En estos algoritmos, el sistema operativo (a través de una interrupción de reloj del hardware) actúa como un árbitro estricto. Se le asigna a cada proceso un fragmento de tiempo máximo (llamado **Quantum**). Si el proceso no ha terminado cuando su Quantum se agota, **el SO interrumpe al proceso a la fuerza**, lo devuelve a la cola de "Listos" y le entrega la CPU a otro proceso. 

También puede ocurrir si llega a la cola un proceso con una prioridad mucho mayor (ej. una alerta del sistema) que el que se está ejecutando actualmente.

*   **Ventajas:** Garantizan la **responsividad** y la equidad. Un programa colgado no puede congelar tu PC; el SO simplemente le quita la CPU a la fuerza y sigue ejecutando el resto de tus programas. Te permite escuchar música, mover el ratón y descargar un archivo simultáneamente de forma fluida.
*   **Desventajas:** Generan mucha más **sobrecarga (overhead)** debido a que el SO realiza miles de cambios de contexto por segundo. Además, complican el desarrollo: si dos procesos comparten datos y uno es interrumpido a la mitad de una escritura, los datos pueden corromperse (requiriendo semáforos o bloqueos mutuos).
*   **Ejemplos:** Round-Robin (asignación por turnos circulares), SRTF (Shortest Remaining Time First) o colas multinivel.
*   **Contexto moderno:** Todos los SO modernos de escritorio y móviles (Windows 10/11, Linux, macOS, Android, iOS) utilizan planificadores fuertemente apropiativos para dar la ilusión de que todo se ejecuta al mismo tiempo.