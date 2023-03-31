#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

void* funcao_thread(void* p){
    long index = (long) p;
    printf("Sou a thread %ld\n", index);
    sleep(3);
    printf("Tchau\n");
    exit(0);
}

int main(){
    pthread_t threads[5];

    for(long i = 0; i < 5; i++){
        pthread_create(&threads[i], NULL, funcao_thread, (void*)i);
    }

    for(int i = 0; i < 5; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}