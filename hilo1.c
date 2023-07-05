#include <pthread.h>
#include <stdio.h>

int contador = 0;
main(){
    void *funcionThread(void *parametro);
    pthread_t idHilo;
    int error;
    error = pthread_create(&idHilo,NULL, funcionThread,NULL);
    if (error != 0) {
        perror("No se puede crear hilo...");
    }
    while(1){
        contador++;
    printf("Padre: %d\n",contador);
    }
}

void *funcionThread(void *parametro){
    while(1){
        contador--;
        printf("Hilo: %d\n",contador);
    }
}
