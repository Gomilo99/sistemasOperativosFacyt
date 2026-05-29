---
creado: 16/05/2026
modificado: 18/05/2026
alias:
tags:
  - sistemas_operativos
  - estudio
tipo: Concepto
base:
  - "[[A-Sistemas Operativos]]"
  - "[[04-Recursos]]"
---
# Notas de libro para Asignación N°1: Repaso

## EJECUCIÓN DE INSTRUCCIONES
La tarea básica que realiza un computador es la ejecución de los programas. El programa a ejecutar consta de un conjunto de instrucciones almacenadas en memoria. El procesador lleva a cabo el trabajo, ejecutando las instrucciones especificadas en el programa.

Para alcanzar una mayor comprensión de esta función y de la manera en que los componentes principales del computador interactúan para ejecutar un programa, hace falta analizar con cierto detalle los elementos de la ejecución de un programa. EL punto de vista más sencillo es considerar que el procesamiento de instrucciones consta de dos pasos. El procesador (1) trae las instrucciones desde la memoria, una cada vez y (2) ejecuta cada instrucción. La ejecución de un programa consiste en la repetición de este proceso de lectura y ejecución de la instrucción. La ejecución de la instrucción puede involucrar varias operaciones y depende de la naturaleza de la instrucción.
![[SO-Stallings-instrucciones_img1.png| 500]]

El procesamiento requerido para una instrucción simple se llama ciclo de instrucción. El ciclo de instrucción se representa en la figura 1.2, empleándose esta descripción simplificada de dos pasos que se acaba de explicar. Los dos pasos se llaman ciclo de lectura (fetch) y ciclo de ejecución. La ejecución del programa se detiene sólo si se apaga la máquina, ocurre algún tipo de error irrecuperable o se encuentra una instrucción en el programa que detiene el computador.


### Lectura y ejecución de instrucciones
Al comienzo de cada ciclo de instrucción, el procesador lee una instrucción de la memoria. En un procesador típico habrá un registro llamado *contador de programa* (PC), que se usa para llevar la cuenta de cuál es la próxima instrucción a leer. A menos que se diga otra cosa, el procesador siempre incrementará el PC después de leer cada instrucción, de forma que después se lea la instrucción siguiente en la secuencia (es decir, la instrucción ubicada en la dirección inmediatamente superior de la memoria). Así pues, considérese por ejemplo un computador en la que cada instrucción ocupa una palabra de memoria de 16 bits. Supóngase que el contador de programa apunta a la ubicación 300. La próxima instrucción que va a leer el procesador es la que está en la ubicación 300. En los siguientes ciclos de instrucción, leerá las instrucciones de las ubicaciones 301, 302, 303 y así sucesivamente. Como se verá en breve, esta secuencia puede alterarse.

La instrucción leída se carga en un registro del procesador conocido como *registro de instrucción* (IR). La instrucción está en forma de código binario que especifica cuál es la acción que el procesador llevará a cabo. El procesador interpreta la instrucción y realiza la acción requerida. En general, estas acciones pueden clasificarse en las siguientes cuatro categorías:
- **Procesador-memoria**: Se transfieren datos del procesador a la memoria o viceversa.
- **Procesador-EIS**: Se transfieren datos desde o hacia un dispositivo periférico, realizándose la transferencia entre el procesador y un módulo de E/S.
- **Tratamiento de datos**: El procesador realiza alguna operación aritmética o lógica sobre los datos,
- **Control**: La instrucción pide que se altere la secuencia de ejecución. Por ejemplo, el procesador puede leer una instrucción de la ubicación 149, la cual especifica que la próxima instrucción sea la de la ubicación 182. El procesador recordará este hecho ajustando el valor del contador de programa a 182. De este modo, en el próximo ciclo de lectura, se traerá la instrucción de la ubicación 182 y no de la 150, 

Por supuesto que la ejecución de una instrucción puede incluir una combinación de estas acciones. 

Como ejemplo sencillo, se considera una máquina hipotética que incluya las características enumeradas en la figura 1.3. El procesador contiene un único registro de datos, llamado *acumulador* (AC). Tanto las instrucciones como los datos son de 16 bits de longitud. Así pues, es conveniente organizar la memoria utilizando ubicaciones o palabras de 16 bits. El formato de instrucción dedica cuatro bits para el código de la operación (cod-op). Por tanto, pueden haber hasta $2^{4}$ = 16 códigos de operación diferentes y hasta $2^{12}$ = 4096 (4K) palabras de memoria que se pueden direccionar directamente. ![[SO-Stellings-instrucciones_img2.png]]

La figura 1.4 ilustra la ejecución parcial de un programa, mostrando las zonas pertinentes de la memoria y los registros del procesador. El fragmento de programa que se muestra suma el contenido de la palabra de memoria de la dirección 940 al contenido de la palabra de memoria de la dirección 941 y almacena el resultado en esta última dirección. Se requieren tres instrucciones, que se pueden describir con tres ciclos de lectura y tres de ejecución: 
![[SO-Stellings-instrucciones_img3.png]]
1. El PC contiene 300, la dirección de la primera instrucción. Se carga el contenido de la ubicación 300 en el IR. Nótese que este proceso podría involucrar el uso de un MAR y un MBR. Por simplicidad, se van a ignorar estos registros intermedios.
2. Los primeros 4 bits del IR indican que se cargará el AC. Los 12 bits restantes especifican la dirección, que es 940.
3. Se incrementa el PC y se lee la instrucción siguiente,
4. El contenido anterior del AC y el contenido de la ubicación 941 se suman y el resultado se almacena en el AC.
5. Se incrementa el PC y se lee la instrucción siguiente.
6. El contenido del AC se almacena en la ubicación 941.
En este ejemplo se necesitan tres ciclos de instrucción, donde cada uno consta de un ciclo de lectura y otro de ejecución, para sumar el contenido de la ubicación 940 al contenido de la ubicación 941. Con un conjunto de instrucciones más complejo harían falta menos ciclos. La mayoría de los procesadores actuales aportan instrucciones que incluyen más de una dirección. De esta manera, en el ciclo de ejecución de una instrucción particular pueden participar más de una referencia a memoria. Además, en vez de referencias a memoria, una instrucción puede especificar una operación de E/S.
### Funciones de E/S
Hasta ahora se ha discutido que el funcionamiento del computador es controlado por el procesador y se ha analizado en primer lugar la interacción entre el procesador y la
memoria. En la discusión sólo se ha aludido al papel de la componente de E/S.

Los módulos de E/S (un controlador de disco, por ejemplo) pueden intercambiar datos directamente con el procesador. Al igual que el procesador puede iniciar una lectura o escritura en la memoria, indicando la dirección de una ubicación específica, el procesador también puede leer datos de un módulo de E/S o escribir datos en el módulo. En este último caso, el procesador identifica a un dispositivo específico que es controlado por un módulo de E/S determinado. De este modo se podría tener una secuencia de instrucciones similar a la de la figura 1.4, pero con instrucciones de E/S en lugar de referencias a memoria.

En algunos casos, es conveniente permitir que los intercambios de E/S se produzcan directamente con la memoria. En tal caso, el procesador dará autoridad a un módulo de E/S para leer o escribir en memoria, de modo que la transferencia de E/S ocurre sin obstruir al procesador. Durante la transferencia, el módulo de E/S emite órdenes de lectura o escritura en la memoria, librando de responsabilidades al procesador en el intercambio. Esta operación se conoce como *acceso directo a memoria* (DMA, *Direct Memory Access*) y será examinada más adelante en este mismo capítulo. Por ahora, todo lo que se necesita saber es que la estructura de interconexión del computador puede que tenga que permitir una interacción directa entre la memoria y la E/S.