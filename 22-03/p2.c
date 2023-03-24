#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int r_fork = fork();

    if(r_fork == 0){
        printf("clonado\n");
        // execlp();
    }

    return 0;
}