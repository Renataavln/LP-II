/*
(Questão 3 - 4,0)
Faça um programa em C/C++ que implemente uma solução para o problema do buffer compartilhado.
Com um buffer compartilhado de 64 chars, o Programa deverá implementar duas funções:
 
	- produtor() - gera um caractere alfanumérico aleatório e o deposita
	no buffer; exibe o valor e a posição no buffer na tela; dorme 300ms;
	repete;
 
	- consumidor() - captura um dos valores disponíveis no buffer 
	compartilhado e armazena em um arquivo; dorme um tempo aleatório 
	entre 50ms e 200ms; repete;
 
A função produtor deverá ser realizada por uma thread; já a função consumidor deverá ser realizada
por três (5) threads. Cada consumidor armazena as informações em um buffer diferente (de tamanho 64)
e todas as threads devem armazenar todos os valores gerados pelo produtor. 
 
Use semáforos (biblioteca semaphore.h) para sincronizar o acesso ao buffer  compartilhado, de forma
que todos os números gerados pelo produtor sejam consumidos uma vez, evitando qualquer problema ao
ler ou escrever no buffer. Quando o buffer do consumidor estiver completo, o programa deverá exibir
o seu contaúdo. Quando o buffer for consumido 2 vezes (ou seja, quando o produtor tiverem gerado 128
caracteres e estes forem consumidos por TODOS os consumidores) finalize o programa.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define SIZE_BUFFER 64
#define QTD_CONSUMIDOR 5

char buffer[SIZE_BUFFER] = {0}; // esse é o buffer que é preenchido pelo produtor e lido pelos consumidores
char consumidor_buffer[QTD_CONSUMIDOR][SIZE_BUFFER] = {0}; // cada consumidor tem seu próprio buffer pessoal

// como o front controla onde o consumidor está no buffer e o consumo é independente, foi necessário criar
// um array para que cada consumidor tenha seu front
int front[QTD_CONSUMIDOR] = {0};

// como o rear é usado somente pelo produtor para saber sua localização no buffer, e só tem 1 produtor,
// então só precisa de um rear
int rear = 0;

// foi necessário criar um semaforo 'empty' e um 'full' para cada thread pois elas funcionam independentemente
sem_t empty[QTD_CONSUMIDOR], full[QTD_CONSUMIDOR];
// empty = inicializa com o numero do buffer pois o buffer inicia vazio. O produtor só produz quando o empty está livre
// full = incializa com 0 pois o buffer inicia vazio. O consumidor só consome quando o full está livre

void* produtor(void* p){
	int contador = 0; // contador para verificar se ja foi gerado os 128 caracteres

	while(contador < (SIZE_BUFFER*2)){ // SIZE_BUFFER*2 ---> 64*2 = 128
		for(int i = 0; i < QTD_CONSUMIDOR; i++){
            // ele aguarda todos empty ficarem livres
            sem_wait(&empty[i]);
        }

		char caracter = (rand() % 26) + 'A'; // gerando o caracter aleatório
		buffer[rear] = caracter; // adicionando o caracter na posição do rear
		printf("[P] Produzi %c e armazenei em buffer[%d]\n", caracter, rear);
		
        rear = (rear + 1) % SIZE_BUFFER;
        // incrementando o valor do rear
        // fazer '% SIZE_BUFFER' pq quando rear = 64 ele fica 0, ai reinicia o buffer

		usleep(300);

        for(int i = 0; i < QTD_CONSUMIDOR; i++){
            // libera o full para todos os consumidores
            sem_post(&full[i]);
        }

		contador++;
	}

	printf("[P] Terminei de produzir\n");
	return NULL;
}

void* consumidor(void* p){
	long indice = (long) p; // indice necessário para controle dos prints no terminal
    int consumido = 0; // variável para contar se já foram consumidos 128 caracteres
    front[indice] = 0; // inicialização do front respectivo como 0

	while(consumido < (SIZE_BUFFER*2)){
        // aguarda o produtor liberar o full de seu indice
		sem_wait(&full[indice]);
        // inicio da seção critica relacionada ao indice
		char caracter = buffer[front[indice]]; // salva o caracter do buffer do front respectivo
        consumidor_buffer[indice][front[indice]] = caracter; // salva no seu buffer pessoal
		printf("[C-%ld] Consumi %c armazenado em buffer[%d]\n", indice, caracter, front[indice]);
        int intervalo = (rand() % 151) + 50;
		usleep(intervalo);

        if(front[indice] == SIZE_BUFFER-1){
            // verifica se o front que ele tá é o ultimo lugar do buffer, se sim então o buffer
            // está cheio, então ele printa tudo
            printf("[C-%ld] Buffer cheio: ", indice);
            for(int i = 0; i < SIZE_BUFFER; i++){
                printf("%c ", consumidor_buffer[indice][i]);
            }
            printf("\n");
        }

		front[indice] = (front[indice] + 1) % SIZE_BUFFER;
        // incrementando valor do front
        // fazer '% SIZE_BUFFER' pq quando front = 64 ele fica 0, ai reinicia o buffer

		consumido++;
        // fim da seção critica relacionada ao indice
        sem_post(&empty[indice]);
	}

	printf("[C-%ld] Terminei de consumir\n", indice);
}

int main(){
	pthread_t t_prod, t_cons[QTD_CONSUMIDOR];
    time_t t;

    srand((unsigned) time(&t));

    for(int i = 0; i < QTD_CONSUMIDOR; i++){
        // inicializando o empty com o tamanho do buffer pois o buffer está vazio
        sem_init(&empty[i], 0, SIZE_BUFFER);

        // inicializando o full com 0 pois o buffer está vazio
        sem_init(&full[i], 0, 0);
    }

	pthread_create(&t_prod, NULL, produtor, NULL); // criando a thread do produtor
	for(long i = 0; i < QTD_CONSUMIDOR; i++){
        // criando as threads dos consumidores
		pthread_create(&t_cons[i], NULL, consumidor, (void*)i);
	}

	pthread_join(t_prod, NULL); // dando join no produtor
	for(int i = 0; i < QTD_CONSUMIDOR; i++){
        // dando join nos consumidores
		pthread_join(t_cons[i], NULL);
	}

    printf("[main] Fim do programa\n");

	return 0;
}