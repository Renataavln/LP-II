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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define SIZE_BUFFER 32
#define QTD_CONSUMIDOR 3

char buffer[SIZE_BUFFER] = {0};
int front = 0, rear = 0;
int consumido = 0;
sem_t empty, full, mutex;

void* produtor(void* p){
	int contador = 0;
	time_t t;
	srand(time(&t));

	while(contador < 128){
		sem_wait(&empty);
		char caracter = (rand() % 26) + 'A';
		buffer[rear] = caracter;
		printf("[P] Produzi %c e armazenei em buffer[%d]\n", caracter, rear);
		rear = (rear + 1) % SIZE_BUFFER; // fazer isso pq quando rear = 32 ele fica 0 ai reinicia
		usleep(150);
		sem_post(&full);
		contador++;
	}

	printf("[P] Terminei de produzir\n");
	return NULL;
}

void* consumidor(void* p){
	long indice = (long) p;
	time_t t;
	srand(time(&t));

	while(consumido < 128){
		sem_wait(&full);
		char caracter = buffer[front];
		buffer[front] = '0';
		printf("[C-%ld] Consumi %c armazenado em buffer[%d]\n", indice, caracter, front);
		front = (front + 1) % SIZE_BUFFER; // fazer isso pq quando rear = 32 ele fica 0 ai reinicia
		int intervalo = (rand() % 71) + 30;
		usleep(intervalo);

		sem_wait(&mutex);
		consumido++;
		sem_post(&mutex);

		sem_post(&empty);
	}

	printf("[C-%ld] Terminei de consumir\n", indice);
	exit(0);
}

int main(){
	sem_init(&empty, 0, SIZE_BUFFER);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
	pthread_t t_prod, t_cons[QTD_CONSUMIDOR];

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