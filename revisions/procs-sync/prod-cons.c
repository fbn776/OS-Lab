#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 20

int nums[SIZE];
int f = -1, r = -1;

void Enqueue(int nums[], int *f, int *r, int item) {
	if(*f == (*r + 1) % SIZE) {
		printf("Queue is full\n");
	} else {
		if(*f == -1)
			*f = 0;
		(*r) = (*r + 1) % SIZE;
		nums[*r] = item;
	}
}

int Dequeue(int nums[], int *f, int *r) {
	if(*f == -1) {
		printf("Queue is empty\n");
		return -1;
	}

	int item = nums[*f];
	*f = (*f + 1) % SIZE;
	if(*f = (*r + 1) % SIZE)
		*f = *r = -1;

	return item;
}

void wait(int *s) {
	while(*s <= 0);
	(*s)--;
}

void signal(int *s) {
	(*s)++;
}

int i = 0;
int empty = SIZE, full = 0, mutex = 1;
void *producer(void *args) {
	while(i < 20) {
		wait(&empty);
		wait(&mutex);

		sleep(1);
		printf("Produced - %d\n", i);
		Enqueue(nums, &f, &r, i);
		i++;

		signal(&mutex);
		signal(&full);
	}
}

void *consumer(void *args) {
	while(1) {
		wait(&full);
		wait(&mutex);

		int item = Dequeue(nums, &f, &r);
		printf("Consumed: %d\n", item);
		if(item % 2 == 0)
			printf("Even\n");
		else
			printf("Odd\n");

		signal(&mutex);
		signal(&empty);
	}
}


int main() {
	pthread_t p1, p2;
	pthread_create(&p1, NULL, producer, (void *) NULL);
	pthread_create(&p1, NULL, consumer, (void *) NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	return 0;
}
