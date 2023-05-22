#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

int global_counter = 0;

void* job_function(void* p){
    long index = (long)p;
    
    printf("[%ld] job() - start\n", index);
    // simula job
    sleep(((int)index)+5);
    printf("[%ld] job() - finished\n", index);
    //
    __atomic_fetch_add(&global_counter, 1, 0);
    while(global_counter != QTD_THREADS);
    printf("[%ld] passed barrier\n", index);    
}

int main(){
    pthread_t threads[QTD_THREADS];
    
    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, job_function, (void*)i);
    }

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("[main] finishing...\n");

    return 0;
}