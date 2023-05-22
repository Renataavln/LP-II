#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("Antes\n");
    int r = fork();

    if(r==0){
        printf("clonado [%d]\n", getpid());
        printf("pid do original: %d\n", getppid());
        while (1);
    }else{
        printf("original\n");
        printf("pid do clonado: %d\n", r);
        printf("pid do original: %d\n", getpid());
    }

    return 0;
}