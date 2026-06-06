---
alias:
tags:
  - clase
  - sistemas_operativos
creado: 04/06/2026
modificado: 04/06/2026
tipo: Concepto
base:
  - "[[A-Sistemas Operativos]]"
  - "[[04-Recursos]]"
---
# Sincronización y Comunicación entre procesos
## Sincronización
La sincronización es un conjunto de protocolos y mecanismos utilizados para preservar la integridad y consistencia del sistema, cuando varios procesos concurrentes comparten recursos que son reutilizables en serio (sólo pueden ser utilizados por un proceso a la vez). Su estado y operación pueden resultar corrompidos si son manipulados concurrentemente y sin sincronización por más de un procesos. Ej: variables compartidas para Lectura-Escritura, dispositivos físicos como impresoras, etc.

La **sincronización** se define como un subconjunto de protocolos y mecanismos diseñados para **preservar la integridad y consistencia del sistema** cuando múltiples procesos concurrentes comparten recursos que deben utilizarse de forma serializada (uno a la vez). Sin estos mecanismos, el estado y la operación de los recursos compartidos, como variables de lectura/escritura o dispositivos físicos (impresoras), podrían corromperse si se manipulan simultáneamente.
- **Propósito y Cooperación**: La sincronización permite que dos o más procesos cooperen mediante el uso de señales. Esto permite obligar a un proceso a **detenerse en una posición determinada** hasta que reciba una señal específica que le permita continuar coordinando así sus actividades.
- **Relación con la Comunicación**: La comunicación entre procesos se considera una herramienta fundamental para la sincronización, ya que permite coordinar las distintas tareas que realizan.
- **Problemas que resuelve**: Ayuda a gestionar la **concurrencia**, la cual presenta desafíos como la competencia por recursos, la necesidad de exclusión mutua y la prevención de condiciones de carrera (donde el resultado depende del orden de ejecución).
- **Herramientas comunes**:
	- **Semáforos**: Variables especiales que se utilizan para la señalización entre procesos mediante operaciones atómicas de espera (*wait*) y la señal (*signal*).
	- **Monitores**: Estructuras de lenguajes de programación que encapsulan variables y procedimientos, utilizando **variables de condición** para suspender y reanudar procesos según sea necesario.
	- **Paso de mensajes**: Un método que ofrece funciones de comunicación y sincronización, permitiendo a los procesos intercambiar información y coordinarse incluso en sistemas distribuidos.
- **Diferencia con la Exclusión Mutua**: Mientras que la exclusión mutua asegura que solo un proceso acceda a un recurso a la vez, la sincronización es un concepto más amplio que abarca la **coordinación de la ejecución y el tiempo** entre los procesos para asegurar que la interacción sea correcta.