#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void* swapper(void*);

int arr[2];
sem_t mtx;

int main(int argc, char* argv[]) {
    pthread_t who;
    long int loop;
// Request (called once)
    sem_init(&mtx,
  0, /* Process share mode (0:for threads  non-zero:for processes */
  1  /* initial value */);

    // get value of loop var (from command line arg)
    loop = atoi(argv[1]);

    arr[0] = 0;
    arr[1] = 1;
    pthread_create(&who, NULL, swapper, &loop);
    int temp;
    // swap array
    for (int k = 0; k < loop; k++) {
        sem_wait(&mtx);
        temp = arr[0]; // need temp variable to swap
        arr[0] = arr[1];
        arr[1] = temp;
        sem_post(&mtx);
    }
    int rc;
    pthread_join(who, (void **) &rc);
    printf ("Values: %5d %5d\n", arr[0], arr[1]);
    sem_destroy(&mtx);

}

void* swapper(void *arg) {
    int temp;
    long int *num = (long int *) arg;
    for (int k = 0; k < *num; k++) {
        sem_wait(&mtx);
        temp = arr[0]; // need temp variable to swap
        arr[0] = arr[1];
        arr[1] = temp;
        sem_post(&mtx);

    }
    return 0;
}
