#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 5

typedef struct {
    int sleepTime;
    int i;
} PhiloArgs;

pthread_mutex_t chopstick[SIZE];

void* philo(void *vargs) {
    PhiloArgs *data = (PhiloArgs*)vargs;

    printf("Thread %d is ready\n", data->i);
    while(1) {
        pthread_mutex_lock(&chopstick[data->i]);
        pthread_mutex_lock(&chopstick[(data->i + 1) % SIZE]);

        printf("\n\nPhilo-%d is eating...\n", data->i);
        sleep(2);
        printf("Philo-%d stopped eating...\n", data->i);

        pthread_mutex_unlock(&chopstick[data->i]);
        pthread_mutex_unlock(&chopstick[(data->i + 1) % SIZE]);

        sleep(data->sleepTime);
    }
    return NULL;
}

int main() {
    pthread_t threads[SIZE];
    PhiloArgs args[SIZE];

    // Initialize mutexes
    for (int i = 0; i < SIZE; i++) {
        pthread_mutex_init(&chopstick[i], NULL);
    }

    // Create threads
    for (int i = 0; i < SIZE; i++) {
        args[i].i = i;
        args[i].sleepTime = rand() % 5 + 1; // Random sleep time between 1 and 5 seconds
        pthread_create(&threads[i], NULL, philo, (void*)&args[i]);
    }

    // Join threads
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy mutexes
    for (int i = 0; i < SIZE; i++) {
        pthread_mutex_destroy(&chopstick[i]);
    }

    return 0;
}
