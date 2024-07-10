#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

int empty = BUFFER_SIZE; // Semaphore indicating empty slots in buffer
int full = 0;            // Semaphore indicating full slots in buffer
int mutex = 1;           // Semaphore for mutual exclusion

void wait(int *s) {
    while (*s <= 0) ;   // Busy wait while semaphore is zero
    (*s)--;             // Decrement semaphore
}

void signal(int *s) {
    (*s)++;             // Increment semaphore
}

void produce(int item) {
    wait(&empty);       // Wait if buffer is full
    wait(&mutex);       // Acquire mutex for critical section
    
    // Produce item and add to buffer
    buffer[in] = item;
    printf("Produced item %d at position %d\n", item, in);
    in = (in + 1) % BUFFER_SIZE;
    
    signal(&mutex);     // Release mutex
    signal(&full);      // Signal that buffer is now full
}

int consume() {
    int item;
    wait(&full);        // Wait if buffer is empty
    wait(&mutex);       // Acquire mutex for critical section
    
    // Consume item from buffer
    item = buffer[out];
    printf("Consumed item %d from position %d\n", item, out);
    out = (out + 1) % BUFFER_SIZE;
    
    signal(&mutex);     // Release mutex
    signal(&empty);     // Signal that buffer is now empty
    
    return item;
}

void *producer(void *arg) {
    int i;
    for (i = 1; i <= 10; i++) {
        produce(i);     // Produce 10 items
        sleep(1);       // Simulate some processing time
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int i, item;
    for (i = 1; i <= 10; i++) {
        item = consume();   // Consume 10 items
        sleep(2);           // Simulate some processing time
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    return 0;
}
