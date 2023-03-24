#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int loop_count = 0;
    int count = 0;

    printf("Paramentros recebidos: [%d]\n", argc);
    for(int i = 0; i < argc; i++){
        printf("[%d] %s\n", i, argv[i]);
    }
    
    if(argc > 1){
        loop_count = atoi(argv[1]);
    }

    while(count < loop_count){
        // printf("loop\n");
        usleep(100);
        count++;
    }

    printf("[loop] fora do while\n");

    return 0;
}