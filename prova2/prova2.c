/*
(Questão 2 - 5,0)
Escreva um programa em C/C++ que simule o funcionamento de um sistema de pedágio em uma rodovia.
O programa deve usar threads para representar os veículos que passam pelo pedágio e deve explorar a
solução do problema da seção crítica (solucionando com trava ou semáforo) e sincronismo condicional (usando barreira).
 
Cada veículo deve escolher aleatoriamente uma das cabines de pedágio para passar e deve esperar sua vez
para pagar o pedágio. Quando um veículo paga o pedágio, ele deve incrementar o valor total arrecadado pelo
pedágio. O programa deve garantir que apenas um veículo por vez possa pagar o pedágio em cada cabine
(usando trava ou semáforo para solucionar o problema da seção crítica).
 
Além disso, o programa deve usar uma barreira para sincronizar os veículos de forma que eles só possam
sair do pedágio depois que todos os veículos tenham pago o pedágio. Depois que todos os veículos saírem
do pedágio, o programa deve imprimir o valor total arrecadado pelo pedágio.
 
O programa deve terminar quando todos os veículos tiverem passado pelo pedágio.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define QTD_PEDAGIO 3
#define QTD_VEICULOS 5

int pedagio = 0;

pthread_barrier_t barreira;
sem_t sem_cabine[QTD_PEDAGIO]; // criei um semaforo para cada cabine
sem_t sem_pedagio; // semaforo responsável só por cuidar da variável 'pedagio'

void* func_veiculo(void* p){
    long indice = (long) p; // preciso do indice para fazer o controle dos prints na tela

    int cabine = rand() % QTD_PEDAGIO; // 'cabine' armazena a cabine escolhida aleatoriamente pelo veiculo
    
    // a thread está esperando a cabine escolhida ser liberada
    sem_wait(&sem_cabine[cabine]);
    // início da seção critica da cabine
    printf("[%ld] Entrei na cabine %d\n", indice, cabine);

    int taxa = (rand() % 100) + 1; // valor do pedagio

    // a thread está esperando a variável 'pedagio' ser liberada por outra thread
    sem_wait(&sem_pedagio);
    // início da seção crítica do pedagio
    pedagio += taxa;
    // fim da seção crítica do pedagio
    sem_post(&sem_pedagio); // liberou o pedágio para outra thread

    printf("[%ld] Paguei %d\n", indice, taxa);

    // fim da seção critica da cabine
    sem_post(&sem_cabine[cabine]); // liberou a cabine para outra thread
    
    // chegada da thread na barreira
    pthread_barrier_wait(&barreira);
    // saída da thread na baerreira

    return NULL;
}

int main(){
    pthread_t t_veiculos[QTD_VEICULOS];
    pthread_barrier_init(&barreira, NULL, QTD_VEICULOS+1);
    // barreira inicializada em 'QTD_VEICULOS+1' porque a barreira só é liberada quando
    // os veiculos (QTD_VEICULOS) e a main (+1) chegam na barreira

    sem_init(&sem_pedagio, 0, 1); // semaforo do pedagio é inicializado em 1
    for(int i = 0; i < QTD_PEDAGIO; i++){
        // como criei um array de semaforos para a cabine então eu preciso que de um for para inicializar todos com 1
        sem_init(&sem_cabine[i], 0, 1);
    }

    time_t t;

    srand((unsigned) time(&t));

    for(long i = 0; i < QTD_VEICULOS; i++){
        // criando as threads
        pthread_create(&t_veiculos[i], NULL, func_veiculo, (void*)i);
    }

    // chegada da main na barreira
    pthread_barrier_wait(&barreira);
    // saída da main na barreira

    printf("Total do pedágio = %d\n", pedagio);

    return 0;
}