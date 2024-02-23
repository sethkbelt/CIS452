nclude <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>

typedef struct {
    int arr[2];
} shared_info;

int main (int argc, char*argv[]) {
    int status;
    long int i, loop = 0;
    shared_info *shr;
    int shmId;
    char shmName[50];
    pid_t pid;
    sprintf(shmName, "swap-%d", getuid());

    /*
     * TODO: get value of loop variable(from command - line
     * argument
     */
    loop = atoi(argv[1]);

    shmId = shm_open (shmName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shmId, sizeof(shared_info));
    shr = mmap(NULL, sizeof(shared_info), PROT_READ|PROT_WRITE, MAP_SHARED, shmId, 0);

    shr->arr[0] = 0;
    shr->arr[1] = 1;

    pid = fork ();
    if (pid == 0) {
        for (i = 0; i < loop; i++) {
        sem_wait(&mtx);
        temp = arr[0]; // need temp variable to swap
        arr[0] = arr[1];
        arr[1] = temp;
        sem_post(&mtx);
        }
        munmap (shr->arr, 2 * sizeof(long int));
        exit (0);
    }
    else {
        for (i = 0; i < loop; i++) {
        sem_wait(&mtx);
        temp = arr[0]; // need temp variable to swap
        arr[0] = arr[1];
        arr[1] = temp;
        sem_post(&mtx);
        }
    }

    wait (&status);
    printf ("values: %d\t%d\n", shr->arr[0], shr->arr[1]);
    munmap (shr->arr, sizeof(shared_info));
    shm_unlink(shmName);
    return 0;
}
