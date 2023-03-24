#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("antes\n");

    int r = fork();
    if(r > 0){
        fork();
    }
    while (1)
    
    return 0;
}