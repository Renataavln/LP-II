#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_SLEEP_TIME 5

int main(){
    int qtd_proc;
    int pid_original;

    pid_original = getpid();

    printf("Digite um número inteiro: ");
    scanf("%d", &qtd_proc);
    printf("Criando %d processos...\n", qtd_proc);
    fflush(stdout);

    for(int i = 0; i < qtd_proc; i++){
        if(getpid() == pid_original){
            fork();
            if(getpid() != pid_original){
                printf("[CLONADO]");
                printf("pid = %d\n", getpid());
                int r_sleep;
                time_t t;
                srand(time(&t));
                r_sleep = rand() % MAX_SLEEP_TIME;
                r_sleep++;

                printf("[Clone] %ds\n", r_sleep);
                sleep(r_sleep);
                printf("[Clone] fim\n");
            }
        }
    }

    if(getpid() == pid_original){
        printf("[ORIGINAL]\n");
        for(int i = 0; i < qtd_proc; i++){
            wait(NULL);
        }
        printf("Original saindo\n");
    }

    return 0;
}