#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define QTD_THREADS 5

typedef struct {
    int id;
    char categoria;
    float valor;
} item_t;

void* funcao_thread(void* data){
    item_t* meu_item = (item_t*)data;

    printf("[thread-%d] %c %f\n", meu_item->id, meu_item->categoria, meu_item->valor);
}

int main(){
    pthread_t threads[QTD_THREADS];
    item_t itens[QTD_THREADS];
    time_t t;

    srand((unsigned) time(&t));

    for(int i = 0; i < QTD_THREADS; i++){
        itens[i].id = (i+1);
        itens[i].categoria = (char)(65+i);
        itens[i].valor = (float) ((float)(rand() % 1000) + ((float) ((float)(rand() % 100) / 100)));
        printf("[%d] %d %c %f\n", (i+1), itens[i].id, itens[i].categoria, itens[i].valor);
    }

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, funcao_thread, (void*)&itens[i]);
    }

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}