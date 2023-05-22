#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

/*
(Questão Extra) Faça um programa em C/C++ que implemente uma solução
para o problema do buffer compartilhado. Com um buffer compartilhado de 32 
chars, o Programa deverá implementar duas funções:

	- produtor() - gera um caractere alfanumérico aleatório e o deposita
	no buffer; exibe o valor e a posição no buffer na tela;	dorme 150ms;
	repete;

	- consumidor() - captura um dos valores disponíveis no buffer 
	compartilhado e armazena em um arquivo; dorme um tempo aleatório 
	entre 30ms e 100ms; repete;

A função produtor deverá ser realizada por uma thread; já a função consumidor
deverá ser realizada por três (3) threads. Cada consumidor armazena
as informações em um arquivo diferente. Ao final, a thread principal deve 
consolidar todos os arquivos em um só (use barreiras).

Use semáforos (biblioteca semaphore.h) para sincronizar o acesso ao buffer 
compartilhado, de forma que todos os números gerados pelo produtor sejam
consumidos uma vez, evitando qualquer problema ao ler ou escrever no buffer.
Quando o buffer for consumido 4 vezes (ou seja, quando os produtores tiverem
gerado 128 caracteres e estes forem consumidos) finalize o programa, salvando
o conteúdo em arquivos. Comente o código explicando o uso dos semáforos.

Referências para manipulação de arquivos em C:

https://www.inf.pucrs.br/~pinho/LaproI/Arquivos/Arquivos.htm (português)
http://www.csc.villanova.edu/~mdamian/C/c-files.htm (inglês)
*/

#define BUFFER_SIZE 32
#define QTD_CONS 3

char buffer[BUFFER_SIZE];
int front = 0, rear = 0;
int consumido = 0;
sem_t empty, full, mutex;

void* funcao_produtor(void* p) {
    int gerado = 0;
    while (1) {
        char c = 'A' + rand() % 26;
        sem_wait(&empty);
        buffer[rear] = c;
        rear = (rear + 1) % BUFFER_SIZE;
        printf("[P] %c (%d)\n", c, rear);
        gerado++;
        usleep(150);
        sem_post(&full);
        if (gerado == 128) {
            printf("[P] Fim do produtor\n");
            pthread_exit(0);
        }
    }
    return NULL;
}

void* funcao_consumidor(void* p) {
    long id = (long)p;
    while (1) {
        sem_wait(&full);
        char c = buffer[front];
        front = (front + 1) % BUFFER_SIZE;
        printf("[C-%ld] %c (%d)\n", id, c, front);
        //salvaria o caracter em arquivo
        int dormir = 30 + rand() % 71;
        sem_wait(&mutex);
        consumido++;
        if (consumido >= 128) {
            printf("[C-%ld] Fim do consumidor\n", id);
            exit(0);
        }
        sem_post(&mutex);
        usleep(dormir);
        sem_post(&empty);
    }
     return NULL;
}

int main(void) {
    pthread_t produtor;
    pthread_t consumidores[QTD_CONS];
    time_t t;

    srand((unsigned) time(&t));

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&produtor, NULL, funcao_produtor, NULL);

    for (long i = 0; i < QTD_CONS; i++) {
        pthread_create(&consumidores[i], NULL, funcao_consumidor, (void*)i);
    }

    pthread_join(produtor, NULL);

    for (int i = 0; i < QTD_CONS; i++) {
        pthread_join(consumidores[i], NULL);
    }

    return 0;
}