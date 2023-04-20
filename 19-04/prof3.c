#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

long var_global = 0;
int lock = 0;

void* funcao_thread(void* args){
    long index = (long) args;
    int counter = 0;

    while(counter < 1000){
        counter++;  
        printf("[t-%ld] antes da SC\n", index);

        while(__atomic_test_and_set(&lock, 0));

        //início seção critica
        var_global++;
        printf("[t-%ld] %ld\n", index, var_global);
        //fim seção critica
        lock = 0;
        printf("[t-%ld] depois da SC\n", index);
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