#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

int arrive[QTD_THREADS] = {0};
int proceed[QTD_THREADS] = {0};
int global_counter = 0;

void* job_function(void* p){
    long index = (long)p;
    
    printf("[%ld] job() - start\n", index);
    // simula job
    sleep(((int)index)+5);
    printf("[%ld] job() - finished\n", index);
    arrive[index] = 1;
    printf("[%ld] arrive barrier\n", index);    
    while(proceed[index] != 1);
    printf("[%ld] passed barrier\n", index);
    proceed[index] = 0;
}

void* coord_func(void* p){
    for(int i = 0; i < QTD_THREADS; i++){
        while(arrive[i] != 1);
        arrive[i] = 0;
    }

    for(int i = 0; i < QTD_THREADS; i++){
        proceed[i] = 1;
    }
}

int main(){
    pthread_t threads[QTD_THREADS];
    pthread_t t_coord;
    
    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, job_function, (void*)i);
    }
    pthread_create(&t_coord, NULL, coord_func, NULL);

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("[main] finishing...\n");

    return 0;
}