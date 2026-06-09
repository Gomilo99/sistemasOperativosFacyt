// 06-productor_consumidor_unbounded.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mutex.h"
#include "semaphore.h"

// Buffer implementado como lista enlazada (sin tamaño fijo)
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* head = NULL;     // frente del buffer (de donde consume el consumidor)
Node* tail = NULL;     // final del buffer (donde agrega el productor)

semaphore sem_mutex;   // control de acceso al buffer
semaphore sem_full;    // elementos disponibles (inicial = 0)

void* productor(void* arg) {
    for (int i = 0; i < 10; i++) {
        usleep(rand() % 200000);
        // Crear nuevo nodo
        Node* nuevo = (Node*)malloc(sizeof(Node));
        nuevo->data = i;
        nuevo->next = NULL;

        wait(&sem_mutex);
        // Añadir al final de la lista
        if (head == NULL) {
            head = tail = nuevo;
        } else {
            tail->next = nuevo;
            tail = nuevo;
        }
        printf("Productor: produce %d\n", i);
        signal(&sem_mutex);
        signal(&sem_full);   // avisa que hay un elemento más
    }
    return NULL;
}

void* consumidor(void* arg) {
    for (int i = 0; i < 10; i++) {
        wait(&sem_full);     // espera al menos un elemento
        wait(&sem_mutex);

        // Extraer del frente
        Node* nodo = head;
        int item = nodo->data;
        head = head->next;
        if (head == NULL) tail = NULL;
        free(nodo);
        printf("Consumidor: consume %d\n", item);

        signal(&sem_mutex);
        usleep(rand() % 200000);
    }
    return NULL;
}

int main() {
    printf("=== PRODUCTOR-CONSUMIDOR CON BUFFER NO ACOTADO ===\n");
    init(&sem_mutex, 1);
    init(&sem_full, 0);

    ThreadPair hilos = crear_procesos(productor, consumidor);
    esperar_procesos(hilos);

    destroy(&sem_mutex);
    destroy(&sem_full);
    printf("\nTerminado.\n");
    return 0;
}