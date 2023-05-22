#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define QTD 5

sem_t semaforo;

void* thread_func(void* p){
    long id = (long) p;

    printf("[%ld] Aguardando...\n", id);

    sem_wait(&semaforo);                  // protocolo de entrada
    printf("[%ld] Dentro da SC\n", id);   // seção critica
    sleep(2);                             // seção critica
    sem_post(&semaforo);                  // protocolo de saída
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[QTD];
    sem_init(&semaforo, 0, 1);            // precisa inicializar o semaforo antes de usar

    for(long i = 0; i < QTD; i++){
        pthread_create(&threads[i], 0, thread_func, (void*)i);
    }

    for(int i = 0; i < QTD; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}