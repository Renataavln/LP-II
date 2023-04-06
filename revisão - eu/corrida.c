#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define PULO_MAX 10

typedef struct{
    int indice;
    int tamanho_pista;
} t_lebres;


void* lebres_funcao(void* p){
    t_lebres* lebre = (t_lebres*) p;

    int total = 0;

    while(total < lebre->tamanho_pista){
        sleep(1);
        time_t t;
        srand(time(&t)*lebre->indice);
        int pulo = rand() % PULO_MAX+1;
        total += pulo;

        printf("Lebre %d pulou %dm [Total = %d]\n", lebre->indice, pulo, total);
    }

    printf("Lebre %d ganhou!\n", lebre->indice);

    exit(0);
}

int main(int argc, char* argv[]){
    if(argc != 4){
        printf("Quantidade inválida de argumentos\n");
        exit(0);
    }

    if(!strcmp(argv[1], "-t")){ // Se for thread
        int quantidade = atoi(argv[2]);
        int tamanho_pista = atoi(argv[3]);
        pthread_t* threads_lebres = (pthread_t*)malloc(sizeof(pthread_t)*quantidade);
        t_lebres* lebres = (t_lebres*)malloc(sizeof(t_lebres)*quantidade);

        for(int i = 0; i < quantidade; i++){
            lebres[i].indice = i;
            lebres[i].tamanho_pista = tamanho_pista;
            pthread_create(&threads_lebres[i], NULL, lebres_funcao, (void*)&lebres[i]);
        }

        for(int i = 0; i < quantidade; i++){
            pthread_join(threads_lebres[i], NULL);
        }

        free(threads_lebres);
        free(lebres);
    }else if(!strcmp(argv[1], "-p")){ // Se for processo
        int quantidade = atoi(argv[2]);
        int tamanho_pista = atoi(argv[3]);
        int orig_pid = getpid();
        int* clones_pid = (int*)malloc(sizeof(int)*quantidade);
        
        for(int i = 0; i < quantidade; i++){
            if(getpid() == orig_pid){
                clones_pid[i] = fork();

                if(clones_pid[i] == 0){
                    int total = 0;

                    while(total < tamanho_pista){
                        sleep(1);
                        time_t t;
                        srand(time(&t)*i);
                        int pulo = rand() % PULO_MAX+1;
                        total += pulo;

                        printf("Lebre %d pulou %dm [Total = %d]\n", i, pulo, total);
                    }

                    printf("Lebre %d ganhou!\n", i);
                    exit(0);
                }
            }
        }

        if(getpid() == orig_pid){
            wait(NULL);

            for(int i = 0; i < quantidade; i++){
                kill(clones_pid[i], SIGHUP);
            }
        }

        free(clones_pid);
    }else{
        printf("Argumento inválido\n");
    }

    return 0;
}