#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;      // Mutex para sincronización
pthread_cond_t cond_main;   // Variable de condición para la función main
pthread_cond_t cond_hilos;  // Variable de condición para los hilos

int hilos_listos = 0;  // Contador de hilos listos
int main_listo = 0;    // Variable para indicar si la función main está lista

void *funcion01(void *arg) {
    int i;
    for (i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);
        printf(".");
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    pthread_mutex_lock(&mutex);
    hilos_listos++;  // Incrementar el contador de hilos listos
    pthread_cond_signal(&cond_hilos);  // Despertar a la función main
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *funcion02(void *arg) {
    int i;
    for (i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);
        printf("o");
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    pthread_mutex_lock(&mutex);
    hilos_listos++;
    pthread_cond_signal(&cond_hilos);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *funcion03(void *arg) {
    int i;
    for (i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);
        printf("+");
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    pthread_mutex_lock(&mutex);
    hilos_listos++;
    pthread_cond_signal(&cond_hilos);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t hilo1, hilo2, hilo3;
    int i;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_main, NULL);
    pthread_cond_init(&cond_hilos, NULL);

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

    pthread_mutex_lock(&mutex);
    while (hilos_listos < 3) {
        pthread_cond_wait(&cond_hilos, &mutex);  // Esperar a que los hilos estén listos
    }
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    for (i = 0; i < 20; i++) {
        printf("x");
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
        sleep(3);

        pthread_mutex_lock(&mutex);
    }

    main_listo = 1;  // Marcar la función main como lista
    pthread_condbroadcast(&cond_main);  // Despertar a todos los hilos para finalizar
    pthread_mutex_unlock(&mutex);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    pthread_join(hilo3, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_main);
    pthread_cond_destroy(&cond_hilos);

    exit(0);
}
