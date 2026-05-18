---
creado: 23/12/2025
modificado: 18/05/2026
alias:
tags:
  - sistemas_operativos
  - estudio
  - Global
tipo: Concepto
base:
  - "[[A-Sistemas Operativos]]"
  - "[[04-Recursos]]"
---
# CONTENIDO
**TEMA 1. Evolución de los Sistemas Operativos y Conceptos Básicos.**
Definición de los Sistemas Operativos. Dos Visiones de los Sistemas Operativos: Máquina Extendida, Controlador de Recursos. Funciones de los Sistemas Operativos. Evolución Histórica. Tipos de Sistemas Operativos: Monolíticos, Sistemas de Capas, Máquinas Virtuales, Multiprocesadores y Distribuidos.
**TEMA 2. Administración, Control y Planificación de Procesos en Sistemas Centralizados, Distribuidos y Multiprocesadores.**
- **Descripción y Planificación de procesos:** Concepto de Proceso. Estados de un Proceso. Transición de Estados de un Proceso. Implantación de Procesos (PCB). Concepto de Hilos. Estado de los Hilos. Hilos a Nivel Usuario. Hilos a Nivel de Núcleo. Planificación de Procesos: Concepto de Planificación. Tipos de Planificadores, Algoritmos de Planificación, Análisis de Rendimiento.
- **Comunicación y sincronización entre procesos – Interbloqueos:** Comunicación entre Procesos: Condiciones de Competencia, Secciones Críticas, Recursos Críticos. Principios de Concurrencia, Exclusión Mutua y Sincronización: Algoritmo de Dekker, Test and Set, Semáforos, Contadores de Eventos, Monitores, Paso de Mensajes. Interbloqueos: Definición, Principios, Condiciones, Detección, Prevención, Recuperación, Inanición.
**TEMA 3. Administración de la Memoria.**
Concepto y evolución de la organización del almacenamiento. Concepto y funciones del Administrador de la memoria. Asignación Contigua de la memoria: Monoprogramación, Particionamiento Estático, Swapping, Reubicación Dinámica, Particionamiento Dinámico, Compactación, Compartición de Datos y Programas. Asignación no contigua: Concepto y Gestión de Memoria Virtual, Paginación, Tabla de Páginas, Algoritmos de reemplazo de páginas, Modelación de algoritmos de paginación, aspectos de diseño e implantación para los sistemas de paginación, segmentación, segmentación-paginada. Políticas: Lectura, Ubicación, Reemplazo, Gestión del Conjunto Residente, Vaciado y Control de Carga. Protección de Memoria. Carga y Enlace, carga absoluta, carga relocalizable, carga dinámica en tiempo de ejecución.
**TEMA 4. Administración de Dispositivos.**
Concepto y funciones del Administrador de Dispositivos. Componentes del Administrador de Dispositivos. Dispositivos de Entrada / Salida. Procesamiento de una Operación de Entrada/ Salida. Fase de detección, compilación, ejecución y finalización de una operación de E/S. Almacenamiento Intermedio. Planificación de Discos. Consideraciones de Rendimiento.
**TEMA 5. Administrador de Información.**
Concepto y funciones del administrador de información. Archivos: concepto, estructura, tipos, métodos de acceso, atributos y operaciones. Directorios: rutas de acceso, operaciones, estructura de directorios. Implantación de archivos y directorios. Archivos compartidos. Gestión del almacenamiento secundario. Gestión de archivos en UNIX y Windows. Confiabilidad y desempeño de los sistemas de archivo. Protección de sistemas informáticos. Protección de archivos: objetivos, mecanismos de protección, modelos y problemas de protección. Seguridad: Ataques genéricos, virus, principios de diseño. Autenticación del usuario. Criptografía: concepto, tipos y algoritmos.

# Temario

| Instrumento         | Contenido                                                                                                                                                                     | %   |
| ------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --- |
| Examen Parcial I    | Tema 1: Evolución y Conceptos Básicos<br>Tema 2a: Descripción y Planificación de procesos<br>Tema 2b: Sincronización y Comunicación entre procesos e interbloqueos<br>Tema 3: | 40  |
| Examen Parcial II   | Tema 4: Administración de Dispositivos<br>Tema 5: Administración de Información<br>                                                                                           | 35  |
| Proyecto            | Implementación de Algoritmos                                                                                                                                                  | 15  |
| Asignaciones/Taller | Entrega de asignaciones. Asistencia y participación en las sesiones presenciales. Taller evaluado                                                                             | 10  |
# Planificación 2025-2026

| Semana                                                 | Actividades                                                                                                    | Entregables - Evaluación                                                                                                                                                                                       |
| ------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 11-15 mayo                                             | Presentación Plan de trabajo<br>Clase: Fundamentos de SO                                                       | Documento planificación y evaluación aprobado por consenso<br>Asignación N°1. Repaso Arquitectura del Computador                                                                                               |
| 18-22 mayo<br>25-29 mayo<br>01-05 junio<br>08-12 junio | Clases: <br>- Gestión de Procesos. <br>- Planificación<br>- Sincronización y Comunicación. <br>- Interbloqueos | Asignaciones:<br>- N°2. Planificación de Procesos<br>- N°3. Gestión de Procesos                                                                                                                                |
| 15-19 junio<br>22-26 junio<br>29/06-03 julio           | Clases Gestión Memoria                                                                                         | Asignación N°4. Memoria.<br>Envío del Proyecto Sincronización de procesos en C                                                                                                                                 |
| **06-10 julio**                                        | **Parcial I**                                                                                                  | **Tema 1: Evolución y Conceptos Básicos<br>Tema 2a: Descripción y Planificación de procesos<br>Tema 2b: Sincronización y Comunicación entre procesos e interbloqueos<br>Tema 3: Administración de la Memoria** |
| 20-24 julio<br>27-31 julio                             | Clases Gestión de E/S                                                                                          | Asignación N°5. Gestión E/S                                                                                                                                                                                    |
| 03-07 agosto<br>10-14 agosto                           | Clases Gestión de Archivos                                                                                     | Asignación N°6. Gestión de Archivos                                                                                                                                                                            |
| **17-21 agosto**                                       | **Parcial II**                                                                                                 | **Tema 4: Administración de Dispositivos<br>Tema 5: Administración de Información**                                                                                                                            |
| 12-17 octubre                                          | Taller<br>Defensa del Proyecto<br>Revisión de evaluaciones                                                     | Evaluación del Proyecto<br>Entrega de calificaciones finales                                                                                                                                                   |
# Bibliografía
CARRETERO, J.; García F.; De Miguel, P.; Pérez F. Sistemas Operativos: Una Visión Aplicada. McGraw-Hill. Ed 2001.
CARRETERO, J.; García F.; Pérez F. Prácticas de Sistemas Operativos. De la Base al Diseño. McGraw-Hill. Ed 2001
CARRETERO, J.; García F.; Pérez F. Problemas de Sistemas Operativos: De la Base al Diseño. McGraw-Hill. Edición 2001.
TANEMBAUM, A. Sistemas Operativos Modernos. Prentice Hall HispanoAmericana S.A. 1992.
TANEMBAUM, A. Sistemas Operativos Distribuidos. Prentice Hall Hispanoamericana S.A. 1996.
STALLINGS, W. Principios de Diseño e Interioridades. Prentice Hall – Madrid. Cuarta Edición. 2001.
STALLINGS, W. Sistemas Operativos. Prentice Hall – Madrid. Segunda Edición. 1997.
MILENKOVIC, M. Sistemas Operativos. Mc Graw Hill. Segunda Edición. 1994.
PETERSON, J.; Silberschatz, A. Operating System Concepts. Addison Wesley, 1985.
OTROS ENLACES DE INTERÉS
https://cs.uns.edu.ar/~so/index.php?opmp=apuntes
http://ocw.uc3m.es/ingenieria-informatica/sistemas-operativos/material-de-clase