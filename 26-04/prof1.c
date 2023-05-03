#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* barreira - forma geral
    process Worker[i =1 to n] {
        while(true){
            job();
            ponto de embarreiramento;

        }
    }
*/

#define QTD_THREADS 5

pthread_barrier_t my_barrier;

void* job_function(void* p){
    long index = (long)p;

    int _sleep = (int)index + 1;
    printf("[%ld] job(%d)\n", index, _sleep);
    // simula job
    sleep(_sleep);
    // acabou simula job
    printf("[%ld] finished job(%d) - barrier\n", index, _sleep);
    pthread_barrier_wait(&my_barrier);
    printf("[%ld] left barrier\n", index);
}

int main(){
    pthread_t threads[QTD_THREADS];
    pthread_barrier_init(&my_barrier, 0, QTD_THREADS);

    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, job_function, (void*)i);
    }

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("[main] finished\n");

    return 0;
}