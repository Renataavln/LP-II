/*
(Questão 1 - 5,0)
Escreva um programa em C que simule um sistema de detecção de vazamento em uma represa.
O programa deve usar threads POSIX e espera ocupada para detectar mudanças em uma variável que
representa o nível de água da represa (valor inicial: 1000 litros). O nível de água é modificado
por um conjunto de 5 threads. Cada thread receberá um valor aleatório do quanto incrementará no
nível do reservatório (entre 10 e 100 litros) e dormirá um tempo aleatório (entre 0 e 300ms) entre
cada incremento. Quando o nível de água atingir 5000 litros, essas threads devem ser bloqueadas até
que a thread principal garanta que o nível do reservatório foi reestabelecido para 1000 litros
(e depois dormir 200ms). Deve ser utilizada espera ocupada em todos os casos e funções atômicas
para modificação do nível do reservatório.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define QTD_THREADS 5

int agua = 1000; // água inicializada como 1000
int lock = 0; // lock vai ser a variável responsável por fazer o travamento

void* func_vazamento(void* p){
    long indice = (long)p; // indice é usado para ter o controle de qual thread tá sendo usada

    while(1){
        int valor = (rand() % 91) + 10;
        int intervalo = (rand() % 301);
        while(__atomic_test_and_set(&lock, 0));
        // nesse while ele fica em looping até ele testar o lock = 0, então ele coloca
        // o lock atômicamente em 1, ou seja, antes de outra thread perceber que o lock estava livre

        //início seção critica
        agua += valor; // adiciona o valor gerado na variável 'agua'
        printf("[%ld] Vazou %d ficando no total %d\n", indice, valor, agua);
        //fim seção critica
        lock = 0; // coloca o lock em 0 para que outras threads consigam entrar

        usleep(intervalo);
    }

    return NULL;
}

int main(){
    pthread_t t_vazamentos[QTD_THREADS];
    time_t t;
    srand((unsigned) time(&t));

    for(long i = 0; i < QTD_THREADS; i++){
        // criando as threads
        pthread_create(&t_vazamentos[i], NULL, func_vazamento, (void*)i);
    }

    while(1){
        while(__atomic_test_and_set(&lock, 0));
        // nesse while ele fica em looping até ele testar o lock = 0, então ele coloca
        // o lock atômicamente em 1, ou seja, antes de outra thread perceber que o lock estava livre

        //início seção critica
        if(agua >= 5000){
            // após entrar na seção critica ele vê o estado da água, se tiver maior ou igual a 5000 ele abaixa,
            // se não ele coloca coloca o lock em 0 e libera
            agua = 1000;
            printf("[Main] Retornou água para %d\n", agua);
            usleep(200);
        }
        // fim seção critica
        lock = 0;
    }

    return 0;
}