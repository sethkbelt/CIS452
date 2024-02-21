#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define FOO 4096

int main () {
    int shmId;
    char *shmPtr;

    key_t my_key = ftok("example.c", 12345);

    if ((shmId =
         shmget (my_key, FOO,
                 IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, NULL, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    printf ("value a: %lu\t value b: %lu\n", (unsigned long) shmPtr,
            (unsigned long) shmPtr + FOO);
    if (shmdt (shmPtr) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }

    struct shmid_ds buffer;
    shmctl (shmId, IPC_STAT, &buffer);
    printf("Buffer: %zu\n", buffer.shm_segsz);
    
    printf("Shared Memory Segment ID: %d\n", shmId);
    pause();

    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit (1);
    }

    return 0;
}