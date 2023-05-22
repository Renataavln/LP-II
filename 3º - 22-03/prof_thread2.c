#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

int var_global[QTD_THREADS];

void* funcao_thread(void* args){
    int index = (long) args;

    while (1){
        printf("[t] Sou a thread %d\n", index);
    }
    
}

int main(){
    pthread_t threads[QTD_THREADS];

    printf("programa iniciado\n");
    
    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, funcao_thread, (void*)i);
    }

    // printf("var_global = %d\n", var_global);

    return 0;
}