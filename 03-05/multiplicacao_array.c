#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 4
#define MATRIX_SIZE 4

int matrix_A[MATRIX_SIZE][MATRIX_SIZE] = {{1,2,3,4},
					  {5,6,7,8},
					  {9,10,11,12},
					  {13,14,15,16}};

int matrix_B[MATRIX_SIZE][MATRIX_SIZE] = {{1,0,0,0},
					 {0,1,0,0},
					 {0,0,1,0},
					 {0,0,0,1}};

int matrix_C[MATRIX_SIZE][MATRIX_SIZE] = {0};

pthread_barrier_t barrier;

void* multiplica(void* p) {
	long index = (long)p;
	printf("[%ld] multiplica()\n", index);
	long start = index * MATRIX_SIZE / QTD_THREADS;
	long end = (index+1) * MATRIX_SIZE / QTD_THREADS;

	for (long i = start; i < end; i++) {
		for (long j = 0; j < MATRIX_SIZE; j++) {
			for (long k = 0; k < MATRIX_SIZE; k++) {
				matrix_C[i][j] += matrix_A[i][k] * matrix_B[k][j];
			}	
		}
	}

	printf("[%ld] barrier_wait\n", index);
	pthread_barrier_wait(&barrier);
}

int main(void) {
	pthread_t threads[QTD_THREADS];

	pthread_barrier_init(&barrier, NULL, QTD_THREADS);

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], NULL,
				multiplica, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], NULL);
		//pode exibir parte calculada por thread
	}

	printf("\nMatriz resultante: \n");
	for (int i = 0; i < MATRIX_SIZE; i++) {
		printf("\t|\t");
		for (int j = 0; j < MATRIX_SIZE; j++) {
			printf("%d\t", matrix_C[i][j]);
		}
		printf("\t|\n");
	}

	return 0;
}

