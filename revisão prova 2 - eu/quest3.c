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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

float temperatura = 600;
int contador = 0;
sem_t semaforo;

void* aquece(void* p){
    time_t t;
    srand(time(&t));

    while(contador < 3){
        int valor = (rand() % 101) - 10;

        sem_wait(&semaforo);
        temperatura += valor;
        printf("Temperatura = %.2f\n", temperatura);
        sem_post(&semaforo);

        usleep(200);
    }

    return NULL;
}

void* resfria(void* p){
    while(contador < 3){
        while(temperatura <= 1000);
        usleep(300);
        sem_wait(&semaforo);
        temperatura = 600;
        printf("Temperatura = %.2f\n", temperatura);
        sem_post(&semaforo);

        contador++;
    }

    return NULL;
}

int main(){
    pthread_t t_aquecedor, t_resfriador;
    sem_init(&semaforo, 0, 1);

    pthread_create(&t_aquecedor, NULL, aquece, NULL);
    pthread_create(&t_resfriador, NULL, resfria, NULL);

    pthread_join(t_aquecedor, NULL);
    pthread_join(t_resfriador, NULL);

    return 0;
}