#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

/*
(Questão 3) Faça um programa em C/C++ que simule o controle de temperatura de 
um motor. O programa deverá ter uma variável global do tipo float chamada 
temperatura (que armazenará a temperatura em Celsius do motor, com valor 
inicial 600 C) e duas threads além da main, realizando funções diferentes. 
A primeira thread  realizará   um   incremento   na   temperatura com um valor
aleatório entre -10 e 90 e depois dormirá 200ms. A segunda thread deverá conter
um mecanismo que detecte o quanto antes quando o valor da temperatura 
ultrapassar 1000C - quando isso acontecer deve-se dormir 300ms e o valor da 
variável temperatura passa a ser 600 novamente.
 
O programa deverá ser finalizado depois de 3 reduções de temperatura do motor.
Comente o código explicando o mecanismo utilizado na segunda thread e qual 
seria a alternativa se não houvesse o requisito de detecção da temperatura de
forma mais imediata possível. 
*/

float temperatura = 600;
sem_t semaforo;

void* funcao_aquecer(void* p) {
    while(1) {
        int valor = rand() % 101 - 10;
        sem_wait(&semaforo);
        temperatura += valor;
        //__sync_fetch_and_add(&temperatura, valor);
        //se usar funcao atomica, nao precisa semaforo
        printf("[A] Temperatura: %f\n", temperatura);
        sem_post(&semaforo);
        usleep(200);
    }
}

void* funcao_resfriar(void* p) {
    int reducao = 0;
    while (reducao < 3) {
        while(temperatura < 1000);
        usleep(300);
        //sem_wait(&semaforo);
        temperatura = 600;
        //atribuicao atomica, nao precisa semaforo
        printf("[R] Temperatura: %f\n", temperatura);
        //sem_post(&semaforo);
        reducao++;
    }
    printf("Motor resfriado 3 vezes\nFim do programa\n");
    exit(0);
}

int main(void) {
    time_t t;
    pthread_t thread_aquecer, thread_resfriar;

    srand((unsigned) time(&t));
    sem_init(&semaforo, 0, 1);

    pthread_create(&thread_aquecer, NULL, funcao_aquecer, NULL);
    pthread_create(&thread_resfriar, NULL, funcao_resfriar, NULL);

    pthread_join(thread_aquecer, NULL);
    pthread_join(thread_resfriar, NULL);

}