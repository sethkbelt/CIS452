/****************************************************************************
 * Title: reader.c
 * Author: Seth Konynenbelt
 * Created: February 16, 2024
 * Description: This code exists as a reader to shared memory.
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define FOO 4096

struct shared_data {
    int read_flag1;
    int read_flag2;
    int quit_flag;
    char user_string[4096];
    // todo maybe add a quit flag
};

int main (int argc, char *argv[]) {
    printf("arc %d\n", argc);
    printf("argv: %s\n", argv[1]);
    int which_reader = atoi(argv[1]);
    printf("which_reader %d\n", which_reader);
    struct shared_data *data;
    int shmId;
   // char *shmPtr;

    // generate key to the shared memory
    key_t my_key = ftok("example.c", 12345);

    // shmget creates new shared memory and allows user to write to it
    if ((shmId =
         shmget (my_key, FOO,
                S_IRUSR | S_IWUSR)) < 0) {
        perror ("shmget failed to get memory\n");
        exit (1);
    }

    // attaches the specified shared memory region
    // data is the address of the shared memory region
    if ((data = shmat (shmId, NULL, 0)) == (void *) -1) {
        perror ("Cannot attach to shared memory region\n");
        exit (1);
    }

    // reading the data from the shared memory region
    while(data->quit_flag == 0) 
    {
        if(data->read_flag1 == 0 && which_reader == 1)
            {
                printf("%s", data->user_string);
                data->read_flag1 = 1;
            }
            if(data->read_flag2 == 0 && which_reader == 2)
            {
                printf("%s", data->user_string);
                data->read_flag2 = 1;
            }
        }
    

    // detaches from the shared memory region
    if (shmdt (data) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }

    // // deallocates
    // if (shmctl (shmId, IPC_RMID, 0) < 0) {
    //     perror ("can't deallocate\n");
    //     exit (1);
    // }

    return 0;
}