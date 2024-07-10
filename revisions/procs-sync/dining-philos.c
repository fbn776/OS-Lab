#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 5

typedef struct {
	int sleepTime;
	int i;
} PhiloArgs;

int chopstick[SIZE];

void wait(int *s) {
	while(*s <= 0) ;
	(*s)--;
}

void signal(int *s) {
	(*s)++;
}

void* philo(void *vargs) {

	PhiloArgs *data = (PhiloArgs*)vargs;

	printf("Thread %d is ready\n", data->i);
	while(1) {
		wait(&chopstick[data->i]);
		wait(&chopstick[(data->i + 1) % SIZE]);

		printf("\n\nPhilo-%d is eating...\n", data->i);
		sleep(2);
		printf("Philo-%d stopped eating...\n", data->i);

		signal(&chopstick[data->i]);
		signal(&chopstick[(data->i + 1) % SIZE]);

		sleep(data->sleepTime);
//		printf("i = %d, sleep = %d\n", data->i, data->sleepTime);
	}
}

int main() {
	pthread_t threads[SIZE];
	int i;
	int sleepTimes[5] = {1, 2, 1, 3, 4};

	for(i = 0; i < SIZE; i++)
		chopstick[i] = 1;

	for(i = 0; i < SIZE; i++) {
		PhiloArgs data = {sleepTimes[i], i};
		pthread_create(&threads[i], NULL, philo, (void*) &data);
	}

	for(i = 0; i < SIZE; i++)
		pthread_join(threads[i], NULL);

	return 0;
}
