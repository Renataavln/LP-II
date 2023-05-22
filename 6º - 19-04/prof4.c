#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

/*
int number = 1;
int next = 1;
int turn[QTD_THREADS] = 0;

process CS[i = 1 to n]{
    while(true){
        <turn[i] = number++;>
        <await (turn[i] = next)>
        //seção critica
        <next++;>
    }
}
*/

int number = 1;
int next = 1;
int turn[QTD_THREADS] = {0};
long var_global = 0;

void* funcao_thread(void* p){
    long i = (long) p;
    int counter = 0;

    while(counter < 100000){
        printf("[t-%ld] antes SC\n", i);
        turn[i] = __atomic_fetch_add(&number, 1, 0);
        
        while(turn[i] != next);
        // seção critica
        var_global++;
        // seção critica
        __atomic_fetch_add(&next, 1, 0);
        counter++;
        printf("[t-%ld] depois SC\n", i);
    }
}

int main(){
    pthread_t threads[QTD_THREADS];

    printf("[m] programa iniciando\n");

    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, funcao_thread, (void*)i);
    }
    
    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("[m] var global = %ld\n", var_global);

    return 0;
}