//cria programas novos
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    int o_pid = getpid();
    
    for(int i = 0; i < 5; i++){
        if(getpid() == o_pid){
            printf("[o] Criando processo\n");
            fork();
        }
    }

    if(getpid() != o_pid){
        printf("[c] Olá\n");
    }

    return 0;
}