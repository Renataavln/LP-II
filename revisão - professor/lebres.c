#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define QTD_PARAM 4
#define MAX_PULO 10

int tamanho_pista = 0;
int qtd_lebres = 0;
char concorrencia;

typedef struct {
	int indice;
	int pulo_max;
} t_lebre;

void* lebre_corre(void* p) {	
	int minha_posicao = 0;
	int pulo_corrente = 0;
	t_lebre* l = (t_lebre*) p;
	do {
		//usleep(1000);
		sleep(1);
		pulo_corrente = (rand() % l->pulo_max)+1;
		minha_posicao += pulo_corrente;
		printf("[%d] pulou %d (%d)\n", l->indice, pulo_corrente,
				minha_posicao);
		if (minha_posicao >= tamanho_pista) {
			printf("[%d] venceu!\n", l->indice);
			exit(0);
		}
	} while (minha_posicao < tamanho_pista);
}


int main(int argc, char** argv) {
	time_t t;
	pthread_t* threads;
	t_lebre* lebres;
	int* pids;

	if (argc != QTD_PARAM) {
		printf("use: %s [t/p] <quantidade> <tamanho_pista>\n",
		argv[0]);
		exit(0);
	} else {
			if ((argv[1][0] == 't') || (argv[1][0] == 'p')) {
			concorrencia = argv[1][0];
		} else {
	                printf("use: %s [t/p] <quantidade> <tamanho_pista>\n",
                        argv[0]);
        	        exit(0);
		}	
		qtd_lebres = atoi(argv[2]);
		tamanho_pista = atoi(argv[3]);
	}
	
	srand(time(&t));

	if (concorrencia == 't') {
		printf("Executando corrida com threads!\n");
		printf("Quantidade: %d\n", qtd_lebres);
		printf("Tamanho pista: %d\n", tamanho_pista);
		threads = (pthread_t*) malloc(sizeof(pthread_t)*qtd_lebres);
		lebres = (t_lebre*) malloc(sizeof(t_lebre)*qtd_lebres);

		for (int i = 0; i < qtd_lebres; i++) {
			lebres[i].indice = i+1;
			lebres[i].pulo_max = rand() % MAX_PULO;
			pthread_create(&threads[i], NULL, 
					lebre_corre, 
					(void*)&lebres[i]);
		}

		//pthread_join(threads[0], NULL);
		for (int i = 0; i < qtd_lebres; i++) {
			pthread_join(threads[i], NULL);
		}

	} else if (concorrencia == 'p') {
		int pid_original = getpid();

                printf("Executando corrida com procesos!\n");
                printf("Quantidade: %d\n", qtd_lebres);
		printf("Tamanho pista: %d\n", tamanho_pista);
		lebres = (t_lebre*) malloc(sizeof(t_lebre)*qtd_lebres);
                
		for (int i = 0; i < qtd_lebres; i++) {
                        lebres[i].indice = i+1;
                        lebres[i].pulo_max = rand() % MAX_PULO;
                }

		pids = (int*) malloc(sizeof(int)*qtd_lebres);

		for (int i = 0; i < qtd_lebres; i++) {
			if (getpid() == pid_original) {
				pids[i] = fork();
				if (getpid() != pid_original) {
					lebre_corre((void*)&lebres[i]);
					break;
				} else {
					continue;
				}
			}
		}
		if (getpid() == pid_original) {
			int retval = 0;
			wait(&retval);
				printf("[%d] saindo...\n", retval);
			for (int i = 0; i < qtd_lebres; i++) {
				kill(pids[i], SIGHUP);
			}
			//printf("[main] saindo...\n");
			exit(0);
		}
	}

	return 0;
}
