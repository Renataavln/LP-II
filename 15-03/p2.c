#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int processos;
    int my_pid = getpid();
    int a;
    printf("Quantidade de processos: ");
    scanf("%d", &processos);

    for(int i = 0; i < processos; i++){
        if(getpid() == my_pid){
            a = fork();
        }
    }

    if(a == 0){
        printf("oi\n");
        sleep(15);
    }

    return 0;
}