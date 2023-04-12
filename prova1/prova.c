#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>

/*
    Struct para determinar as variáveis usadas nas threads
    indice: é o indice usado no 'for' de criação
    limite: é o limite máximo do valor aleatorio gerado
*/
typedef struct{
    int indice;
    int limite;
}t_contador;

/*
    Função de uso das threads
*/
void* thread_contador(void* p){
    t_contador* contador = (t_contador*) p; // precisa fazer a conversão
    
    time_t t;
    srand(time(&t)*contador->indice);
    // passando o indice na geração do valor aleatorio para que ele fique mais aleatorio
    long valor = rand() % (contador->limite+1);

    printf("[%d-thread] %ld\n", contador->indice, valor);

    return (void*) valor;
}

int main(int argc, char* argv[]){
    // ./programa 4 3 100
    // ./programa qtdThread qtdProcesso limite

    if(argc != 4){
        printf("Quantidade inválida de argumentos\n");

        exit(0);
    }

    int qtd_thread = atoi(argv[1]);
    int qtd_processo = atoi(argv[2]);
    int limite = atoi(argv[3]);
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*qtd_thread);
    t_contador* contador = (t_contador*)malloc(sizeof(t_contador)*qtd_thread);
    // criando uma struct com os parâmetros para passar para a função das threads
    int original_pid = getpid();
    // salvando o pid do processo original
    int* clones_pid = (int*)malloc(sizeof(int)*qtd_processo);
    // criando um array para salvar os pids dos processos clonados

    // CRIAÇÃO DAS THREADS
    long clock_ini_threads = clock();
    // printf("thread inicio - %ld\n", clock_ini_threads); // testando a saída do clock
    for(int i = 0; i < qtd_thread; i++){
        contador[i].indice = i;
        contador[i].limite = limite;
        pthread_create(&threads[i], NULL, thread_contador, (void*)&contador[i]);
    }

    long thread_exit;
    float soma_threads = 0;
    for(long i = 0; i < qtd_thread; i++){
        pthread_join(threads[i], (void*)&thread_exit);
        // printf("[O-threads] %ld = %ld\n", i, thread_exit); // verificando a saída das threads
        soma_threads += thread_exit;
        // somando a saída das threads
    }
    long clock_fim_threads = clock();
    // printf("thread fim - %ld\n", clock_fim_threads); // testando a saída do clock
    long tempo_threads = (clock_fim_threads - clock_ini_threads) / (CLOCKS_PER_SEC / 1000000);
    // dividindo o clock_per_sec por 1000000 para ficar em microssegundos

    soma_threads = (soma_threads/qtd_thread);
    // calculando a média das saídas das threads

    // CRIAÇÃO DOS PROCESSOS
    long clock_ini_processo = clock();
    // printf("processo inicio - %ld\n", clock_ini_processo); // testando a saída do clock
    for(int i = 0; i < qtd_processo; i++){
        if(getpid() == original_pid){ // apenas o processo com o pid igual ao processo original consegue criar clones
            clones_pid[i] = fork();
            if(clones_pid[i] == 0){ // verificando se o retorno da função fork foi 0
                time_t t;
                srand(time(&t)*getpid());
                // usando o pid do processo para gerar o valor aleatorio
                int valor = rand() % (limite+1);

                printf("[%d-processo] %d\n", i, valor);

                exit(valor);
            }
        }
    }

    int clone_exit;
    float soma_processos = 0;
    for(int i = 0; i < qtd_processo; i++){
        waitpid(clones_pid[i], &clone_exit, 0);
        // printf("[Original-processos] Exit %d: %d\n", i, WEXITSTATUS(clone_exit)); // verificando as saídas dos processos

        soma_processos += WEXITSTATUS(clone_exit);
        // é necessário usar a função 'WEXITSTATUS' para converter a saída do processo clonado
    }
    long clock_fim_processo = clock();
    // printf("processo fim - %ld\n", clock_fim_processo); // testando a saída do clock
    long tempo_processo = (clock_fim_processo - clock_ini_processo) / (CLOCKS_PER_SEC / 1000000);
    // dividindo o clock_per_sec por 1000000 para ficar em microssegundos
    soma_processos = (soma_processos/qtd_processo);
    // calculando a média da soma dos processos

    printf("Threads geradas: %d (%ld microssegundos)\n", qtd_thread, tempo_threads);
    printf("Processos gerados: %d (%ld microssegundos)\n", qtd_processo, tempo_processo);
    printf("Média dos valores gerados pelas threads: %.3f\n", soma_threads);
    printf("Média dos valores gerados pelos processos: %.3f\n", soma_processos);

    free(threads);
    free(contador);
    free(clones_pid);

    return 0;
}