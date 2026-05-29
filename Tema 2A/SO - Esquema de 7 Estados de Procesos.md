---
alias:
tags:
  - sistemas_operativos
  - estudio
creado: 26/05/2026
modificado: 27/05/2026
tipo: Concepto
base:
  - "[[A-Sistemas Operativos]]"
  - "[[04-Recursos]]"
---
## El Proceso
Desde la perspectiva del sistema operativo, un proceso **es un programa en ejecución**. 
- Un programa es una entidad *pasiva* (un archivo con código guardado en disco).
- Un proceso es una entidad *activa*.

### Temas Auxiliares Clave
Para gestionar los procesos y sus cambios de estado, el SO se apoya en los siguientes conceptos:
1. **Bloque de Control de Proceso (*PCB*)**: Es una estructura de datos que el SO utiliza para almacenar toda la información sobre un procesos especifico. Es una estructura en la memoria del *Kernel*. Es esencialmente el "DNI" del proceso. Contiene:
	- *Estado actual* (Listo, Bloqueado, etc.).
	- *Registros del procesador*: Cuando el procesos se pausa, todos los valores matemáticos que la CPU estaba usando se guardan aquí.
	- *Límites de memoria*: Punteros a dónde empieza y termina el proceso en la RAM, para evitar que un proceso lea datos de otro (seguridad).
	- *Archivos abiertos*: Lista de descriptores de archivos que el proceso está usando.
2. **Cambio de Contexto (*Context Switch*)**: Es una operación de **sobrecarga**. Cuando el procesador cambia de un proceso a otro, el SO debe: pausar el proceso A, guardar su estado (contexto) en su PCB, buscar el PCB del proceso B, restaurar el estado en la CPU y saltar a la instrucción de B. Mientras ocurre un cambio de contexto, el sistema no hace ningún trabajo útil (ningún programa avanza).
3. **Planificador (*Scheduler*)**: Es el componente del SO encargado de decidir qué procesos en estado "Listo" debe pasar a ejecutarse en la CPU y por cuánto tiempo. Hay tres tipos de planificadores:
	1. *A corto plazo (CPU Scheduler)*: Mueve procesos de "Listo" a "En ejecución". es rapidísimo y actúa cada poco milisegundos.
	2. *A mediano plazo (Swapper / Intercambiador)*: Se encarga de sacar procesos de la RAM al disco (suspendidos) y traerlos de vuelta para controlar el grado de multiprogramación (cuántos procesos hay en la RAM).
	3. *A largo plazo (Admission Scheduler)*: Decide qué programas nuevos (estado "Nuevo") entran a la cola de "Listos".
4. **Intercambio (*Swapping*)**: Es la técnica de mover procesos completos de la memoria principal (RAM) a la memoria secundaria (disco duro/SSD) y viceversa. 
5. **Memoria de intercambio (*Swap / Pagefile*)**: Es una partición o archivo especial en el disco que el SO trata como si fuera una "extensión" lenta de la memoria RAM.
### Sobrecarga (Overhead)
La sobrecarga es el tiempo de procesamiento, memoria u otros recursos que el sistema gasta haciendo "trabajo administrativo" en lugar de ejecutar código de los programas de usuario. 
>Piensa en un taller mecánico: el tiempo que el mecánico pasa arreglando el motor es el "trabajo útil". El tiempo que pasa limpiando sus herramientas, guardándolas y sacando unas nuevas para el siguiente auto es la **sobrecarga**. Es un trabajo necesario para que el taller funcione bien, pero durante ese tiempo no se está reparando ningún auto.

En el Cambio de contexto de un proceso A y uno B, el SO debe:
1. Pausar A.
2. Guardar todos los valores de A en su PCB (memoria).
3. Buscar a B en la cola de Listos.
4. Cargar los valores del PCB de B en el procesador.
5. Iniciar B.

Todo este procesos toma tiempo (microsegundos o milisegundos). Durante ese breve instante, **la CPU trabaja pero el rendimiento útil para el usuario es nulo**. Si un SO cambia de procesos con demasiada frecuencia, la sobrecarga será tan alta que la computadora se volverá lenta, dedicando más tiempo a administrar los cambios que a ejecutar aplicaciones.
## Los 7 Estados del Proceso
### Origen
**El problema original**: Los primeros sistemas tenían un modelo de 5 estados (Nuevo, Listo, Ejecutando, Bloqueando, Terminado). Sin embargo, las operaciones de E/S (como leer un disco mecánico) son *millones* de veces más lentas que los ciclos de la CPU.

En un sistema ocupado, todos los procesos en RAM podían terminar esperando E/S al mismo tiempo (todos en estado "Bloqueado"). Como resultado, **la CPU se quedaba ociosa (*idle*)**, desperdiciando capacidad de cómputo, y no se podían cargar procesos nuevos porque la RAM estaba llena de procesos bloqueados. 

**La solución**: Añadir el concepto de **Suspensión**. El SO toma a los procesos bloqueados, los copia enteramente al disco duro, libera esa RAM y carga nuevos procesos para mantener a la CPU ocupada. Esto obligó a dividir los estados para saber quién está en RAM y quién en disco duro, naciendo los estados "Bloqueado y Suspendido" y "Listo y Suspendido".

### El Esquema
1. **Nuevo (New)**: El proceso se acaba de crear, pero el SO aún no lo ha admitido en el grupo de procesos ejecutables (típicamente, su código y datos aún no se han cargado en al memoria principal).
2. **Listo (Ready)**: El proceso está cargado en la memoria principal y tiene todos los recursos que necesita para ejecutarse, excepto la CPU. Está en una cola esperando que el planificador del SO le asigne tiempo de procesador.
3. **En ejecución (Running)**: El proceso tiene el control de la CPU y sus instrucciones se están ejecutando en ese momento.
4. **Bloqueado / En espera (Blocked / Waiting)**: El proceso no puede continuar ejecutándose porque está esperando que ocurra un evento externo. Por lo general, esto es una operación de Entrada/Salida (esperar a que el usuario presiona una tecla, leer un archivo del disco, esperar un paquete de red, etc.). Mientras está aquí, el proceso suelta la CPU para que otros puedan usarla.
5. **Terminado (Terminado / Exit)**: El proceso ha terminado su ejecución (ya sea con éxito o por un error) y el SO está liberando sus recursos (memoria, archivos abiertos) y eliminando su PCB.
6. **Bloqueado y Suspendido (Blocked Suspended)**: Si la RAM se llena, el SO puede decidir tomar un proceso que está en estado "Bloqueado" y moverlo al disco duro. El proceso sigue esperando el evento (como lectura de disco), pero ya no ocupa espacio valioso en la RAM.
7. **Listo y Suspendido (Ready Suspended)**: Existen dos formas de llegar a este estado.
	- Un proceso en "Bloqueado y Suspendido" recibe el evento que estaba esperando (por ejemplo, el usuario presionó la tecla). Ya no está bloqueado, está "Listo", pero sigue guardando en el disco duro porque no hay RAM disponible.
	- El SO, por políticas de prioridad o gestión de carga, decide quitar un proceso "Listo" de la RAM y mandarlo al disco para hacerle espacio a un proceso más prioritario.
	Cuando se libera suficiente memoria RAM, un proceso "Listo y Suspendido" es traído de vuelta a la memoria principal, pasando al estado "Listo" normal para competir nuevamente por la CPU.
### Ejemplo Práctico: Corrida en Frío (Dry Run)

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

### Limitaciones, Problemas y Optimizaciones modernas
#### Limitaciones y Problemas
*   **El temido "Thrashing" (Hiperpaginación):** Es el peor escenario del esquema de 7 estados. Ocurre cuando la RAM es tan pequeña que el SO pasa más tiempo moviendo procesos entre la RAM y el disco duro (Swapping) que ejecutándolos en la CPU. El sistema entero se "congela" y el disco duro se pone al 100%.
*   **Latencia extrema:** Si un proceso "Listo y Suspendido" necesita ejecutarse de urgencia, primero debe ser copiado del disco a la RAM. Si el disco es un HDD antiguo, el proceso sufrirá un retraso masivo.

#### Optimizaciones en Sistemas Operativos Modernos (Windows 11, Linux, macOS)
Hoy en día, los SO no siempre mueven procesos *enteros* como sugiere la teoría clásica, sino que usan técnicas más granulares:
1.  **Paginación bajo demanda (Demand Paging):** En lugar de mandar todo el proceso al disco, el SO divide el proceso en "páginas" de 4 Kilobytes. Solo manda al disco las páginas inactivas (por ejemplo, el menú de ayuda del programa que no estás usando), dejando el núcleo del programa en RAM.
2.  **Compresión de Memoria (ZRAM / Memory Compression):** Antes de enviar un proceso al lento disco duro, Windows y macOS intentan *comprimir* los datos en la misma RAM (como si fuera un archivo .zip). Descomprimir en la CPU es mucho más rápido que leer del disco duro.
3.  **SSDs NVMe:** La velocidad extrema de los discos de estado sólido modernos hace que las transiciones hacia y desde los estados "Suspendidos" sean casi imperceptibles para el usuario, enmascarando los cuellos de botella del swapping clásico.

**Tip práctico:** Si abres el Administrador de Tareas en Windows o usas `htop` en Linux, puedes ver esto en acción. En Linux, verás procesos con estado `S` (Sleeping/Bloqueado), `R` (Running), o `D` (Disk Sleep / Bloqueo ininterrumpible por E/S). En Windows, los procesos en estados "Suspendidos" a menudo aparecen con un ícono de una hojita verde o bajo el rubro de "Memoria Comprimida".
