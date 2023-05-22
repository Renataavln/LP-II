#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("alo mundo\n");
    sleep(10);
    int i = 0;
    //for(int i = 0; i < 100000; i++){
        while(1){
            printf("[%d] alo\n", i++);
        }
    //printf("digite x:");
    //scanf("%d", &i);
    
    return 0;
}