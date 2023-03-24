#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int var_global = 0;
int threads_active = 1;

void* funcao1(void* p){
    long var_local_1 = 0;

    while(threads_active){
        var_local_1++;
        printf("[#1] var_local_1 = %ld\n", var_local_1);
        usleep(50);
        var_global++;
        printf("[#1] var_global = %d\n", var_global);
        usleep(50);
    }

    printf("[#1] Finalizando função 1...\n");

    return (void*)var_local_1;
}

void* funcao2(void* p){
    long var_local_2 = 0;

    while (threads_active){
        var_global++;
        printf("[#2] var_global = %d\n", var_global);
        usleep(50);
        var_local_2++;
        printf("[#2] var_local_2 = %ld\n", var_local_2);
        usleep(50);
    }

    printf("[#1] Finalizando função 2...\n");

    return (void*)var_local_2;    
}

int main(void){
    pthread_t thread1, thread2;
    int exec_time = 0;
    int res_1, res_2;

    printf("Digite quantos segundos deseja executar: ");
    scanf("%d", &exec_time);

    pthread_create(&thread1, NULL, funcao1, NULL);
    pthread_create(&thread2, NULL, funcao2, NULL);

    sleep(exec_time);
    printf("[m] Tempo de %d segundos esgotados\n", exec_time);

    threads_active = 0;

    pthread_join(thread1, (void*)&res_1);
    pthread_join(thread2, (void*)&res_2);

    printf("[m] Threads finalizadas\nValor de retorno: %d %d\n", res_1, res_2);
    printf("[m] var_global = %d\n", var_global);
    printf("[m] Saindo...\n");

    return 0;
}