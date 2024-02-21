/*uthor: Seth Konynenbelt

 * Created: February 16, 2024

 * Description: This code exists as a writer and reader to shared memory

 *              using threading

 ***************************************************************************/



/* File include libraries */

#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <sys/ipc.h>

#include <sys/shm.h>

#include <string.h>

#include <pthread.h>

#include <errno.h>

#include <unistd.h>



/* Pre-defined macros */

#define FOO 4096



/* Structue defintions */

struct shared_data

{

    int flag[2];

    int turn;

    int quit_flag;

    char user_string[4096];

};



void *do_greeting3(void *arg);

void *writer_function(void *arg);

void *reader_function(void *arg);



// global (shared and specific) data

int sharedData = 5;

// char val[FOO];



int main()

{



    // decide which reader you are

    // int which_reader = atoi(argv[1]);



    struct shared_data *data;



    int shmId;



    // generate key to the shared memory

    key_t my_key = ftok("example.c", 12345);



    pthread_t thread1, thread2, thread3;

    void *result1, *result2, *result3;

    int status;



    // shmget creates new shared memory and allows user to write to it

    if ((shmId =
             shmget(my_key, FOO,
                    IPC_CREAT | S_IRUSR | S_IWUSR)) < 0)
    {
        perror("shmget failed to get memory\n");
        exit(1);
    }
    printf("shmId %d\b", shmId);
    // attaches the specified shared memory region
    // data is the address of the shared memory region
    if ((data = shmat(shmId, NULL, 0)) == (void *)-1)
    {
        perror("Cannot attach to shared memory region\n");
        exit(1);
    }
    printf("data flag\n");
    data->flag[0] = 0;

    data->flag[1] = 0;

    data->turn = 0;

    data->quit_flag = 12;

    // data->user_string = NULL;

    printf("post] flag\n");



    // create and start two threads executing the "do_greeting3" function

    // pass each thread a pointer to its respective argument

    if ((status = pthread_create(&thread1, NULL, writer_function, &data)) != 0)

    {
        fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
        exit(101);
    }

    if ((status = pthread_create(&thread2, NULL, reader_function, &data)) != 0)

    {
        fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
        exit(101);
    }



    printf("thread create up\n");



    // join with the threads(wait for them to terminate

    //  get their return vals

    if ((status = pthread_join(thread1, &result1)) != 0)

    {
        fprintf(stderr, "join error %d:%s\n", status, strerror(status));
        exit(101);
    }

    if ((status = pthread_join(thread2, &result2)) != 0)
    {
        fprintf(stderr, "join error %d: %s\n", status, strerror(status));
        exit(101);
    }

    printf("thread joined up");
    // detaches from the shared memory region
    if (shmdt(data) < 0)
    {
        perror("just can't let go\n");
        exit(101);
    }



    return 0;

}





void *reader_function(void *data2)

{
    printf("reader \n");
    struct shared_data *data = (struct shared_data *)data2;
    while (1)
    {
        data->flag[1] = 1;
        data->turn = 0;
        while ( data->flag[0] &&  data->turn == 0);
        printf("%s", data->user_string); // for debugging
        data->flag[1] = 0;
   }
    return NULL;
}
void *writer_function(void *data1)
{   
     printf("writer \n");
    struct shared_data *data = (struct shared_data *)data1;

    while (1)
    {
        data->flag[0] = 1;
        data->turn = 1;
        while ( data->flag[1] &&  data->turn == 1);
        fgets(data->user_string, FOO, stdin);
        data->flag[0] = 0;
}
    return NULL;
}
