#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int o_pid = getpid();
    int r = fork();

    if(r > 0){
        printf("Processo original\nPID Original= %d\nPID Clonado = %d\n", o_pid, r);
        sleep(10);
    }else{
        int m_pid = getpid();
        int num;
        printf("Processo clonado\nPID Original= %d\nPID Clonado = %d\n", o_pid, m_pid);

        printf("Insira um número: ");
        scanf("%d", &num);

        printf("Número inserido: %d\n", num);
    }

    return 0;
}