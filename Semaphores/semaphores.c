#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define SIZE 100

int buffer[SIZE];
int f = -1, r = -1;
int i = 0, j = 0, S = 0;

void Enqueue(int Q[], int *front, int *rear, int item) {
	if (*front == ((*rear) + 1) % SIZE)
		printf("Queue is full\n");
	else {
		if (*front == -1)
			*front = 0;
		(*rear) = ((*rear) + 1) % SIZE;
		Q[*rear] = item;
	}
}

int Dequeue(int Q[], int *front, int *rear) {
	if (*front == -1) {
		printf("Queue is empty");
	} else {
		int item = Q[*front];
		*front = ((*front) + 1) % SIZE;

		if (*front == ((*rear) + 1) % SIZE)
			*front = (*rear) = -1;

		return item;
	}
}

void wait(int *S) {
	while (*S <= 0);
	(*S)--;
}

void signal(int *S) {
	(*S)++;
}

void *producer(void *args) {
	do {
		printf("Produced %d\n", i);
		Enqueue(buffer, &f, &r, i++);
		signal(&S);
	} while (i <= 20);
}

void *consumer(void *args) {
	do {
		wait(&S);
		printf("------------Consumed %d\n", Dequeue(buffer, &f, &r));
		j++;
	} while (j <= 20);
}

int main() {
	int pdr_thd_stat, csr_thd_stat;
	pthread_t pdr_thd, csr_thd;

	pdr_thd_stat = pthread_create(&pdr_thd, NULL, producer, (void *) NULL);
	csr_thd_stat = pthread_create(&csr_thd, NULL, consumer, (void *) NULL);


	if (pdr_thd_stat != 0 || csr_thd_stat != 0) {
		perror("Threads error");
		exit(1);
	}

	pthread_join(pdr_thd, NULL);
	pthread_join(csr_thd, NULL);
}
