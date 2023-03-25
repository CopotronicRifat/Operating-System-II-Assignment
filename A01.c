#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 20
#define MAX_VALUE 30

// Shared variables
int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int sum = 0;

// Mutex and condition variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_not_empty = PTHREAD_COND_INITIALIZER;

// Producer function
void *producer(void *args) {
    // Entry section
    for (int i = 1; i <= MAX_VALUE; i++) {
        pthread_mutex_lock(&mutex);
        // Critical section start
        while (((in + 1) % BUFFER_SIZE) == out) {
            pthread_cond_wait(&buffer_not_full, &mutex);
        }
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        pthread_cond_signal(&buffer_not_empty);
        pthread_mutex_unlock(&mutex);
        // Critical section end
    }
    // Exit section
    pthread_exit(NULL);
}

// Consumer function
void *consumer(void *args) {
    int temp_sum = 0;
    // Entry section
    for (int i = 0; i < MAX_VALUE; i++) {
        pthread_mutex_lock(&mutex);
        // Critical section start
        while (in == out) {
            pthread_cond_wait(&buffer_not_empty, &mutex);
        }
        int value = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        temp_sum += value;
        pthread_cond_signal(&buffer_not_full);
        pthread_mutex_unlock(&mutex);
        // Critical section end
    }
    // Reminder section
    pthread_mutex_lock(&mutex);
    sum += temp_sum;
    pthread_mutex_unlock(&mutex);
    // Exit section
    pthread_exit(NULL);
}

int main() {
    pthread_t producer1, producer2, consumer1, consumer2;

    // Create threads
    pthread_create(&producer1, NULL, producer, NULL);
    pthread_create(&producer2, NULL, producer, NULL);
    pthread_create(&consumer1, NULL, consumer, NULL);
    pthread_create(&consumer2, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(producer1, NULL);
    pthread_join(producer2, NULL);
    pthread_join(consumer1, NULL);
    pthread_join(consumer2, NULL);

    printf("Sum = %d\n", sum);

    return 0;
}
