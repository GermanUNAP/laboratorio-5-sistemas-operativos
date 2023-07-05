#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;  // Mutex para la sincronización

void *funcion01(void *arg) {
    int i;
    for (i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);  // Bloquea el mutex
        printf(".");
        fflush(stdout);
        pthread_mutex_unlock(&mutex);  // Desbloquea el mutex
        sleep(1);
    }
    return NULL;
}

void *funcion02(void *arg) {
    int i;
    for (i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);  // Bloquea el mutex
        printf("o");
        fflush(stdout);
        pthread_mutex_unlock(&mutex);  // Desbloquea el mutex
        sleep(1);
    }
    return NULL;
}

void *funcion03(void *arg) {
    int i;
    for (i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);  // Bloquea el mutex
        printf("+");
        fflush(stdout);
        pthread_mutex_unlock(&mutex);  // Desbloquea el mutex
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t hilo1, hilo2, hilo3;
    int i;

    pthread_mutex_init(&mutex, NULL);  // Inicializa el mutex

    printf("Creando hilos...\n");
    if (pthread_create(&hilo1, NULL, funcion01, NULL)) {
        printf("Error al crear hilo 1...\n");
        abort();
    }
    if (pthread_create(&hilo2, NULL, funcion02, NULL)) {
        printf("Error al crear hilo 2\n");
        abort();
    }
    if (pthread_create(&hilo3, NULL, funcion03, NULL)) {
        printf("Error al crear hilo 3\n");
        abort();
    }

    for (i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);  // Bloquea el mutex
        printf("x");
        fflush(stdout);
        pthread_mutex_unlock(&mutex);  // Desbloquea el mutex
        sleep(3);
    }

    // Espera a que los hilos terminen
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    pthread_join(hilo3, NULL);

    pthread_mutex_destroy(&mutex);  // Destruye el mutex

    exit(0);
}
