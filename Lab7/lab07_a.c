#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* swapper(void*);

int arr[2];

int main(int argc, char* argv[]) {
    pthread_t who;
    long int loop;

    // TODO: get value of loop var (from command line arg)

    arr[0] = 0;
    arr[1] = 1;
    pthread_create(&who, NULL, swapper, &loop);
    for (int k = 0; k < loop; k++) {
        // TODO: swap the contents of arr[0] and arr[1]
    }
    int rc;
    pthread_join(who, (void **) &rc);
    printf ("Values: %5d %5d\n", arr[0], arr[1]);
}

void* swapper(void *arg) {
    long int *num = (long int *) arg;
    for (int k = 0; k < *num; k++) {
        // TODO: swap the contents of arr[0] and arr[1]
    }
    return 0;
}
