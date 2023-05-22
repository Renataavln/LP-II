#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

/*
(Questão 2) Faça um programa em C/C++ que controle o acesso de 
threads a duas seções críticas diferentes. O programa deverá ter 10 threads,
e duas variáveis globais (x e y) de tipo int, iniciadas com zero. As threads
modificarão em loop e em sequência as duas variáveis, porém a ordem de 
modificação dependerá do valor do índice que receberão no momento de sua 
criação: caso o índice seja par, a thread incrementa o valor da variável x com 
um valor aleatório entre 5 e 100 e dorme 200ms, em seguida incrementa o valor 
da variável y com um valor aleatório entre 1 e 50 e dorme 100ms; caso o índice 
seja ímpar, a thread incrementa o valor da variável x com um valor aleatório 
entre 1 e 30 e dorme 100ms, em seguida incrementa o valor da variável y com 
um valor aleatório entre 15 e 95 e dorme 250ms.
 
Use semáforos (biblioteca semaphore.h) para controlar o acesso às seções 
críticas, (trate o acesso à cada uma das variáveis como uma seção crítica
diferente) e garanta que todas as  threads  incrementem as variáveis globais
dentro de um ciclo de iterações. O programa deverá ser finalizado quando a 
soma dos valores de x e y for maior que 2000. Comente o código explicando 
o que é uma seção crítica, e como foi feito o controle de acesso às duas 
seções críticas do problema usando semáforos.
*/

int x = 0, y = 0;
sem_t semaforo_x, semaforo_y;

void* funcao_thread(void* p) {
    long id = (long)p;

    while((x+y) <= 2000) {
        if (id % 2 == 0) {
            int valor = rand() % 96 + 5;
            sem_wait(&semaforo_x);
            x += valor;
            printf("Thread %ld incrementou x em %d\n", id, valor);
            sem_post(&semaforo_x);
            usleep(200);
            sem_wait(&semaforo_y);
            valor = rand() % 50 + 1;
            y += valor;
            printf("Thread %ld incrementou y em %d\n", id, valor);
            usleep(100);
            sem_post(&semaforo_y);
        } else {
            int valor = rand() % 30 + 1;
            sem_wait(&semaforo_x);
            x += valor;
            printf("Thread %ld incrementou x em %d\n", id, valor);
            sem_post(&semaforo_x);
            usleep(100);
            valor = rand() % 81 + 15;
            sem_wait(&semaforo_y);
            y += valor;
            printf("Thread %ld incrementou y em %d\n", id, valor);
            usleep(250);
            sem_post(&semaforo_y);
        }
    } 
    return NULL;
}

int main(void) {
    pthread_t threads[10];
    time_t t;  

    srand((unsigned) time(&t));

    sem_init(&semaforo_x, 0, 1);
    sem_init(&semaforo_y, 0, 1);

    for (long i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, 
                        funcao_thread, (void*)i);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("[main] x = %d, y = %d\n", x, y);

    return 0;
}