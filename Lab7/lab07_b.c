#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>

typedef struct {
    int arr[2];
    sem_t mtx;
} shared_info;

int main (int argc, char*argv[]) {
   printf("what happens");
    int status;
    long int i, loop = 0;
    shared_info *shr;
    int shmId;
    char shmName[50];
    pid_t pid;
    printf("fails 1a");
    sprintf(shmName, "swap-%d", getuid());
    printf("fails 1");
    sem_init(&shr->mtx,
  1, /* Process share mode (0:for threads  non-zero:for processes */
  1  /* initial value */);
    /*
     * TODO: get value of loop variable(from command - line
     * argument
     */
    loop = atoi(argv[1]);
    printf("What is going on");
    shmId = shm_open (shmName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shmId, sizeof(shared_info));
    shr = mmap(NULL, sizeof(shared_info), PROT_READ|PROT_WRITE, MAP_SHARED, shmId, 0);

    shr->arr[0] = 0;
    shr->arr[1] = 1;
    int temp;
    pid = fork ();
    if (pid == 0) {
        for (i = 0; i < loop; i++) {
        sem_wait(&shr->mtx);
        temp = shr->arr[0]; // need temp variable to swap
        shr->arr[0] = shr->arr[1];
        shr->arr[1] = temp;
        sem_post(&shr->mtx);
        }
        munmap (shr->arr, 2 * sizeof(long int));
        exit (0);
    }
    else {
        for (i = 0; i < loop; i++) {
        sem_wait(&shr->mtx);
        temp = shr->arr[0]; // need temp variable to swap
        shr->arr[0] = shr->arr[1];
        shr->arr[1] = temp;
        sem_post(&shr->mtx);
        }
    }

    wait (&status);
    printf ("values: %d\t%d\n", shr->arr[0], shr->arr[1]);
    munmap (shr->arr, sizeof(shared_info));
    shm_unlink(shmName);
    sem_destroy(&shr->mtx);
    return 0;
}
