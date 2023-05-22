#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define EXEC_PATH "/home/aluno/lp2/loop"

int main(){
    int fork_return;
    printf("Exemplo exec()\n");

    fork_return = fork();

    if(fork_return < 0){
        printf("fork() error\n");
        exit(0);
    }else if(fork_return == 0){
        char *args[] = {"loop", "100000", NULL};
        printf("[c] Irei executar o programa 'loop'\n");
        int exec_return = execv(EXEC_PATH, args);
        printf("[c] Resultado do exec(): %d\n", exec_return);
    }else{
        int ret_val;
        // waitpid(fork_return, &ret_val, 0);
        sleep(1);
        printf("[o] Clonado finalizado\n");
    }

    return 0;
}