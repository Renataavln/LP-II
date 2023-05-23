/*
1 produtor
5 de buffer
3 consumidores consumindo o buffer inteiro, independente das outras threads
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define SIZE_BUFFER 5
#define QTD_CONSUMIDOR 3

char buffer[SIZE_BUFFER] = {0};
int verificador[SIZE_BUFFER] = {0};
int front[QTD_CONSUMIDOR] = {0};
int rear = 0;
sem_t empty[QTD_CONSUMIDOR], full[QTD_CONSUMIDOR];

void* produtor(void* p){
	int contador = 0;

	while(contador < 15){
		for(int i = 0; i < QTD_CONSUMIDOR; i++){
            sem_wait(&empty[i]);
        }

		char caracter = (rand() % 26) + 'A';
		buffer[rear] = caracter;
		printf("[P] Produzi %c e armazenei em buffer[%d]\n", caracter, rear);
		rear = (rear + 1) % SIZE_BUFFER; // fazer isso pq quando rear = 32 ele fica 0 ai reinicia
		usleep(150);

        for(int i = 0; i < QTD_CONSUMIDOR; i++){
            sem_post(&full[i]);
        }

		contador++;
	}

	printf("[P] Terminei de produzir\n");
	return NULL;
}

void* consumidor(void* p){
	long indice = (long) p;
    int consumido = 0;

	while(consumido < 15){
		sem_wait(&full[indice]);
		char caracter = buffer[front[indice]];
		printf("[C-%ld] Consumi %c armazenado em buffer[%d]\n", indice, caracter, front[indice]);
		front[indice] = (front[indice] + 1) % SIZE_BUFFER; // fazer isso pq quando rear = 32 ele fica 0 ai reinicia
		int intervalo = (rand() % 71) + 30;
		usleep(intervalo);

		consumido++;

        sem_post(&empty[indice]);
	}

	printf("[C-%ld] Terminei de consumir\n", indice);
	exit(0);
}

int main(){
	pthread_t t_prod, t_cons[QTD_CONSUMIDOR];
    time_t t;

    srand((unsigned) time(&t));

    for(int i = 0; i < QTD_CONSUMIDOR; i++){
        sem_init(&empty[i], 0, SIZE_BUFFER);
        sem_init(&full[i], 0, 0);
    }

	pthread_create(&t_prod, NULL, produtor, NULL);
	for(long i = 0; i < QTD_CONSUMIDOR; i++){
		pthread_create(&t_cons[i], NULL, consumidor, (void*)i);
	}

	pthread_join(t_prod, NULL);
	for(int i = 0; i < QTD_CONSUMIDOR; i++){
		pthread_join(t_cons[i], NULL);
	}

	return 0;
}