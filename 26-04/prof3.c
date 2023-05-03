#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

/* barreira - forma geral
    process Worker[i =1 to n] {
        while(true){
            job();
            ponto de embarreiramento;

        }
    }
*/

#define QTD_THREADS 4
#define MAX_REPEAT 5
#define A_ASCI 65

char palavra[QTD_THREADS+1];

pthread_barrier_t palavra_b;
pthread_barrier_t teste_b;

void* job_function(void* p){
    long index = (long)p;
    int counter = 0;

    while(counter < MAX_REPEAT){
        char my_char = ((char)A_ASCI + (rand() % 26));
        printf("[%ld] %c\n", index, my_char);
        palavra[index] = my_char;
        sleep(((int)index+1));
        pthread_barrier_wait(&palavra_b);
        printf("[%ld] passed barrier (palavra)\n", index);
        pthread_barrier_wait(&teste_b);
        printf("[%ld] passed barrier (teste)\n", index);
        counter++;
    }
}

int testa_palindromo(){
    for(int i = 0, j = QTD_THREADS-1; i < QTD_THREADS, j >= 0; i++, j--){
        if(palavra[i] != palavra[j]);
    }

    return 1;
}

int main(){
    time_t t;
    pthread_t threads[QTD_THREADS];
    pthread_barrier_init(&palavra_b, 0, QTD_THREADS+1);
    pthread_barrier_init(&teste_b, 0, QTD_THREADS+1);
    int counter = 0;

    srand(time(&t));
    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, job_function, (void*)i);
    }

    while(counter < MAX_REPEAT){
        printf("[main] aguardando palavra...\n");
        pthread_barrier_wait(&palavra_b);
        palavra[QTD_THREADS] = '\0';
        printf("[main] palavra: %s\n", palavra);
        
        if(testa_palindromo() == 0){
            printf("[main] %s = PALINDROMO\n", palavra);
        }else{
            printf("[main] %s = NÃO É PALINDROMO\n", palavra);
        }

        pthread_barrier_wait(&teste_b);
        counter++;
    }

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&palavra_b);
    pthread_barrier_destroy(&teste_b);

    printf("[main] finished\n");

    return 0;
}