#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/*
(Questão 1) Faça um programa em C/C++ que crie um arquivo de texto que
funcionará como um dicionário contendo todas as strings com 4 caracteres 
alfabéticos maiúsculos (de "AAAA" a "ZZZZ"). Cada letra do alfabeto (de A a
Z) deverá ter sua parte do dicionário gerada por uma thread individual, de tal 
forma que esta thread gere todas as strings iniciadas por aquela letra (ex. a 
thread que gerará as strings começando com a letra 'C' gerará as strings de 
"CAAA" a "CZZZ"). Depois que todas as threads geram suas partes, a thread 
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

pthread_barrier_t barreira;

void* funcao_thread(void* p) {
    char c = (char)p;
    char str[5];
    str[0] = c;
    str[4] = '\0';
    //abre arquivo
    for (int i = 0; i < 26; i++) {
        str[1] = 'A' + i;
        for (int j = 0; j < 26; j++) {
            str[2] = 'A' + j;
            for (int k = 0; k < 26; k++) {
                str[3] = 'A' + k;
                printf("%s\n", str);
                //fprintf (joga no arquivo)
            }
        }
    }
    //fecha arquivo
    pthread_barrier_wait(&barreira);
    //sinalizar barreira que terminou sua parte
    return NULL;
}

int main(void) {
//    funcao_thread((void*)'Z');
    pthread_barrier_init(&barreira, NULL, 27);

    pthread_t threads[26];
    for (int i = 0; i < 26; i++) {
        pthread_create(&threads[i], NULL, funcao_thread, (void*)('A' + i));
    }

    pthread_barrier_wait(&barreira);
/*
    for (int i = 0; i < 26; i++) {
        pthread_join(threads[i], NULL);
    }
*/
    //funcao main consolida o dicionario
    //em um arquivo so a partir dos arquivos gerados
 
    return 0;
}