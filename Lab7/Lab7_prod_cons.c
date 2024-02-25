/****************************************************************************
 * Title: Lab7_prod_cons.c
 * Author: Seth Konynenbelt
 * Created: February 25, 2024
 * Description:
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>

#define LIMIT_SIZE 1000
void *producer();
void *consumer();

typedef struct
{
    int arr[2];
    int n;
} shared_info;
sem_t mtx, empty, full;
int buffer[LIMIT_SIZE];
int bufferSize;
int main(int argc, char *argv[])
{

    // stored consumer/producer information
    int buffer_size = 0, producer_sleep_time = 0, consumer_sleep_time = 0;

    // shared memory variables
    // shared_info *shr;
    int shmId;
    char shmName[50];

    // created thread
    pthread_t thread_producer, thread_consumer;

    if (argc != 4)
    {
        printf("Please enter the valid number of parameters\n");
        exit(99);
    }
    // Buffer size
    buffer_size = atoi(argv[1]);
    // Producer sleep time (in milliseconds)
    producer_sleep_time = atoi(argv[2]);
    // Consumer sleep time (in milliseconds)
    consumer_sleep_time = atoi(argv[3]);

    // start shared memory initialization, 0 for threads, 1 for intialization value
    // Request (called once)
    sem_init(&mtx, 0, 1);
    sem_init(&empty, 0, 0);
    sem_init(&full, 0, buffer_size);

    // create threads
    // thread, , start routine, init value
    pthread_create(&thread_producer, NULL, producer, NULL);
    pthread_create(&thread_consumer, NULL, consumer, NULL);
    // create threads

    // wait for thread to be done
    int rc_1;
    pthread_join(thread_producer, (void **)&rc_1);
    sem_destroy(&mtx);
}
// The program should have three threads (not processes):

// The main (default) thread that initializes all the necessary variables, semaphores and also launches two new worker threads below
// One thread for the producer that "produces" random integers between 1000-9000
// One thread for the consumer

void *producer()
{
    while (1)
    {
        // random number inclusive 1000-9000
        int random_bin = rand() % 9001 + 1000;

        sem_wait(&empty);
        sem_wait(&mtx);

        // entering critical section to add to buffer
        buffer[bufferSize++] = random_bin;
        printf("Producer added %d to bin %d\n", random_bin, bufferSize - 1);

        sem_post(&empty);
        sem_post(&mtx);

        usleep(100000); // TODO make specified time
    }
}

void *consumer()
{
    while (1)
    {

        sem_wait(&full);
        sem_wait(&mtx);

        // entering critical section to remove from buffer
        int item_removed = buffer[--bufferSize];
        printf("\tConsumer took away %d from bin %d\n", item_removed, bufferSize + 1);

        sem_post(&mtx);
        sem_post(&empty);

        usleep(100000); // TODO make specified time

        /* consume the item in next consumed */
    }
}