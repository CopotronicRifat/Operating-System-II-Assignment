/*
Assignment 02
S M Rafiuddin
CWID: A20387294
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Initialize the semaphore for read and write operations
sem_t mutex, wrt;
int rcount = 0, j;
unsigned long data = 0;
int in_cs;

void relaxandspendtime() {
    // Remainder section
    if (in_cs == 1) {
        printf("Warning");
    }
    while (in_cs);
    // Entry section
    if (in_cs == 0) {
        printf("Critical Section has started\n");
        // Critical section
        for (int j = 0; j < 250000000; j++) {
            j = j;
        }
        printf("Critical section has ended\n");
    }
}

// Reader's function
void *reader(void *args) {
    // Entry section
    sem_wait(&mutex);
    rcount++;
    printf("READER %d thread has started its execution\n", rcount);
    // Critical section
    relaxandspendtime();
    printf("READER %d thread has completed its execution\n", rcount);
    // Exit section
    sem_post(&mutex);
}

// Writer's function
void *writer() {
    // Entry section
    sem_wait(&wrt);
    printf("WRITER thread has started its execution.\n");
    in_cs = 1;
    // Critical section
    for (int i = 0; i < 25000; i++) {
        i = i;
    }
    printf("WRITER thread has completed its execution.\n");
    // Exit section
    sem_post(&wrt);
    in_cs = 0;
}

int main(int argc, char *argv[]) {
    int mid, i;
    if (argc <= 1 || argc > 2) {
        printf("Incorrect number of arguments\n");
        exit(1);
    } else {
        j = atoi(argv[1]);
        if (j < 1 || j > 14) {
            printf("Number of READERS is NOT accepted\n");
            exit(1);
        }
    }
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);
    pthread_t rtid[14], wtid;
    mid = (int) j / 2;

    for (i = 0; i < mid; i++) {
        printf("Creating Thread %d\n", i + 1);
        pthread_create(&rtid[i], NULL, reader, NULL);
    }
    printf("Creating WRITER thread\n");
    pthread_create(&wtid, NULL, writer, NULL);
    for (i = mid; i < j; i++) {
        printf("Creating Thread %d\n", i + 1);
        pthread_create(&rtid[i], NULL, reader, NULL);
    }
    printf("Exiting from main loop\n");

    pthread_join(wtid, NULL);
    for (i = 0; i < j; i++) {
        pthread_join(rtid[i], NULL);
    }

    // Destroy the semaphore when the read processes are completed
    sem_destroy(&mutex);
    // Destroy semaphore when the write process is completed
    sem_destroy(&wrt);
}
