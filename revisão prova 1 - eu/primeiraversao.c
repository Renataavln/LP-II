#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define PULO_MAX 30 //30m

int main(int argc, char* argv[]){
    // argv[0] = nome do arquivo
    // argv[1] = -t ou -p (se for t é thread se for p é processo)
    // argv[2] = quantidade de threads/processos
    // argv[3] = tamanho da pista

    if(argc != 4){
        printf("Quantidade inválida de argumentos\n");
    }else{
        int quantidade = atoi(argv[2]);
        int tamanho_pista = atoi(argv[3]);

        if(!(strcmp(argv[1], "-t"))){ //threads
            printf("aaa\n");
        }else if(!(strcmp(argv[1], "-p"))){ //processos
            int origin_pid = getpid();
            int * clones_pid = (int*)malloc(sizeof(int)*quantidade);
            // int clones_pid[quantidade];

            for(int i = 0; i < quantidade; i++){
                if(getpid() == origin_pid){
                    int r = fork();

                    if(r == 0){
                        int contagem = 0;
                        while(contagem < tamanho_pista){
                            time_t t;
                            srand(time(&t));
                            int pulo = rand() % PULO_MAX;
                            pulo++;
                            contagem += pulo;

                            printf("lebre %d saltou %dcm (total: %d)\n", i, pulo, contagem);
                            sleep(1);
                        }
                    }else{
                        clones_pid[i] = r;
                    }
                }
            }

            if(getpid() == origin_pid){
                sleep(10);
                // for(int i = 0; i < quantidade; i++){
                //     printf("%d\n", clones_pid[i]);
                // }
            }

        }else{
            printf("Passagem de argumento inválida\n");
        }
    }

    return 0;
}