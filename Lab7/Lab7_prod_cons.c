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
sem_t mtx, empty, full, scan_mutex;
int buffer[LIMIT_SIZE];
int bufferIndex = 1;

    // stored consumer/producer information
    int buffer_size = 0, producer_sleep_time = 0, consumer_sleep_time = 0;

int main(int argc, char *argv[])
{
    // shared memory variables
    // shared_info *shr;
    // int shmId;
    // char shmName[50];

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

    printf("Buffer size: %d Producer Sleep : %d Consumer Sleep : %d\n", buffer_size, producer_sleep_time, consumer_sleep_time);
    
    // start shared memory initialization, 0 for threads, 1 for intialization value
    // Request (called once)
    sem_init(&mtx, 0, 1);
    sem_init(&empty, 0, 0);
    sem_init(&full, 0, buffer_size);
    sem_init(&scan_mutex, 0, 1);

    // create threads
    // thread, , start routine, init value
    pthread_create(&thread_producer, NULL, producer, NULL);
    pthread_create(&thread_consumer, NULL, consumer, NULL);

    char user_input = '\0';

    while (user_input != 'q') {
        scanf(" %c", &user_input);
        sem_wait(&scan_mutex);

        switch(user_input)
        {
        case'a' :
            producer_sleep_time += 250;
        case'z' :
            producer_sleep_time -= 250;
            if (producer_sleep_time < 0) producer_sleep_time = 0;
        case's' :
            consumer_sleep_time += 250;
        case'x' :
            consumer_sleep_time -= 250;
            if (consumer_sleep_time < 0) consumer_sleep_time = 0;
        case'q' :
            break;
        default :
            printf("Command: |%c| Invalid command. Please enter 'a', 'z', 's', 'x', or 'q'.\n", user_input);
        }
    sem_post(&scan_mutex);

    }
    printf("SHOULD HAVE exited \n");
    // wait for thread to be done
    pthread_join(thread_producer, NULL);
    pthread_join(thread_consumer, NULL);

    sem_destroy(&mtx);
    sem_destroy(&full);
    sem_destroy(&empty);
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

        // if (bufferIndex < LIMIT_SIZE) {
        // entering critical section to add to buffer
        buffer[bufferIndex++] = random_bin;
        printf("Producer added %d to bin %d\n", random_bin, bufferIndex - 1);
        
        sem_post(&mtx);
        sem_post(&full);

        usleep(producer_sleep_time * 1000); // TODO make specified time
    }
    return NULL;
}

void *consumer()
{
    while (1)
    {

        sem_wait(&full);
        sem_wait(&mtx);

        // entering critical section to remove from buffer
        int item_removed = buffer[--bufferIndex];
        printf("\tConsumer took away %d from bin %d\n", item_removed, bufferIndex);

        sem_post(&mtx);
        sem_post(&empty);

        usleep(consumer_sleep_time * 1000); // TODO make specified time
        if (bufferIndex == 0) break;

        /* consume the item in next consumed */
    }
        return NULL;
}