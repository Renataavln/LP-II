#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

int var_global = 0;

void* funcao_thread(void* args){
    var_global++;
    printf("[t] Novo valor de var_global = %d\n", var_global);
}

int main(){
    pthread_t threads[QTD_THREADS];

    printf("programa iniciado\n");
    funcao_thread(NULL);
    printf("var_global = %d\n", var_global);

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, funcao_thread, 0);
    }

    printf("var_global = %d\n", var_global);

    return 0;
}