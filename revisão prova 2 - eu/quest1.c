/*
(Questão 1) Faça um programa em C/C++ que crie um arquivo de texto que
funcionará como um dicionário contendo todas as strings com 4 caracteres 
alfabéticos maiúsculos (de "AAAA" a "ZZZZ"). Cada letra do alfabeto (de A a
Z) deverá ter sua parte do dicionário gerada por uma thread individual, de tal 
forma que esta thread gere todas as strings iniciadas por aquela letra (ex. a 
thread que gerará as strings começando com a letra 'C' gerará as strings de 
"CAAAA" a "CZZZZ"). Depois que todas as threads geram suas partes, a thread 
principal deve gerar um arquivo de texto contendo o dicionário inteiro. 
 
Use barreiras (API Posix Threads - pthread.h ou a API de threads STD C++11) 
para sincronizar as threads.
 
Referências para manipulação de arquivos em C:
 
https://www.inf.pucrs.br/~pinho/LaproI/Arquivos/Arquivos.htm (português)
http://www.csc.villanova.edu/~mdamian/C/c-files.htm (inglês)
 
Comente o código discutindo o ciclo de vida das threads. Através de 
comentários, faça a relação de cada invocação a funções da API PThreads
com a máquina de estados de threads.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
//#include <time.h>

#define QDT_THREADS 26

pthread_barrier_t barreira;

void* func_dicionario(void* p){
    long indice = (long) p;
    char dicionario[5];
    dicionario[0] = ('A' + indice);
    dicionario[4] = '\0';

    for(int i = 0; i < QDT_THREADS; i++){
        dicionario[1] = ('A' + i);
        for(int j = 0; j < QDT_THREADS; j++){
            dicionario[2] = ('A' + j);
            for(int k = 0; k < QDT_THREADS; k++){
                dicionario[3] = ('A' + k);

                printf("%s\n", dicionario);
            }
        }
    }

    pthread_barrier_wait(&barreira);

    return NULL;
}

int main(){
    pthread_t threads[QDT_THREADS];
    pthread_barrier_init(&barreira, NULL, 27);
    
    for(long i = 0; i < QDT_THREADS; i++){
        pthread_create(&threads[i], NULL, func_dicionario, (void*)i);
    }
    // long i = 2;
    // pthread_create(&threads[2], NULL, func_dicionario, (void*)i);

    pthread_barrier_wait(&barreira);

    return 0;
}