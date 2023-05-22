#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("antes\n");
    int x;

    scanf("%d", &x);
    int pid_o = getpid();

    for(int i = 0; i < x; i++){
        if(getpid() == pid_o){
            fork();
        }
    }

    while (1);    

    return 0;
}