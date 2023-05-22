#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 4
#define TAM_ARRAY 8

int soma = 0;

int array[TAM_ARRAY] = {1,2,3,4,5,6,7,8};

//pthread_mutex_t mutex_soma = PTHREAD_MUTEX_INITIALIZER;

void* soma_thread(void* p) {
	long index = (long)p;
	long minha_soma = 0;
	printf("[%ld] soma_thread()\n", index);
	minha_soma = array[2*index] + array[(2*index)+1];
	return (void*)minha_soma;
}

int main(void) {
	pthread_t threads[QTD_THREADS];

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], NULL,
				soma_thread, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		long result_thread = 0;
		pthread_join(threads[i], &result_thread);
		printf("[%d] soma_thread() = %ld\n", i,
				result_thread);
		soma += result_thread;
	}

	printf("Resultado da soma: %d\n", soma);
	
	return 0;
}
