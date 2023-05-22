#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define QTD 2
/* FUNCIONA COM 2 THREADS, MAS COM MAIS DE 2 DÁ ERRADO POIS NÃO DESBLOQUEIA AS OUTRAS THREADS */

sem_t semaforo[QTD];

void* thread_func(void* p){
    long id = (long) p;

    printf("[%ld] job()\n", id);
    sleep(2);
    sem_post(&semaforo[id]);

    for(int i = 0; i < QTD; i++){
        if(id != i){
            sem_wait(&semaforo[i]);
        }
    }

    pthread_exit(NULL);
}

int main(){
    pthread_t threads[QTD];

    for(int i = 0; i < QTD; i++){
        sem_init(&semaforo[i], 0, 0);
    }

    for(long i = 0; i < QTD; i++){
        pthread_create(&threads[i], 0, thread_func, (void*)i);
    }

    for(int i = 0; i < QTD; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}