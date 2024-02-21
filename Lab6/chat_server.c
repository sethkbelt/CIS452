/****************************************************************************
 * Title: chat_server.c
 * Author: Seth Konynenbelt
 * Created: February 1, 2024
 * Description: This file is a chat server with multithreaded processing,
 * in where we aren't allowed to use semaphores or algorthms for
 * concurrency. One can launch N number of chat servers
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
#include <fcntl.h>

/* Pre-defined macros */
#define FOO 4096

/* Structue defintions */
struct shared_data
{
    int read_flag1;
    int read_flag2;
    int quit_flag;
    int write_finish_flag;
    char user_string[4096];
};

void *do_greeting3(void *arg);
void *writer_function(void *arg);
void *reader_function(void *arg);

int main(int argc, char *argv[])
{
    struct shared_data *data;
    int shmId;
    // generate key to the shared memory
    key_t my_key = ftok("example.c", 12345);
    pthread_t thread1, thread2;
    void *result1, *result2;
    int status;

    // shmget creates new shared memory and allows user to write to it
    // if you aren't the first one creating then no "create" macro
    if (argv[1] == 0)
    {
        if ((shmId =
                 shmget(my_key, FOO,
                        IPC_CREAT | S_IRUSR | S_IWUSR)) < 0)
        {
            perror("shmget failed to get memory\n");
            exit(1);
        }
    }
    else
    {
        if ((shmId =
                 shmget(my_key, FOO,
                        S_IRUSR | S_IWUSR)) < 0)
        {
            perror("shmget failed to get memory\n");
            exit(1);
        }
    }

    // attaches the specified shared memory region
    // data is the address of the shared memory region
    if ((data = shmat(shmId, NULL, 0)) == (void *)-1)
    {
        perror("Cannot attach to shared memory region\n");
        exit(1);
    }

    data->quit_flag = 0;
    data->write_finish_flag = 1;
    data->read_flag1 = 1;

    // create and start two threads executing the writer/reader function
    // pass each thread a pointer to its respective argument
    if ((status = pthread_create(&thread1, NULL, writer_function, data)) != 0)
    {
        fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
        exit(101);
    }

    if ((status = pthread_create(&thread2, NULL, reader_function, data)) != 0)
    {
        fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
        exit(101);
    }

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

    // detaches from the shared memory region
    if (shmdt(data) < 0)
    {
        perror("just can't let go\n");
        exit(101);
    }
    return 0;
}

/******************** *reader_function(void *data2) ************************
 *  brief : Reader thread to read shared data
 *  param : data2 - shared data
 *  return: NULL
 ***************************************************************************/
void *reader_function(void *data2)
{
    struct shared_data *data = (struct shared_data *)data2;
    data->quit_flag = 0;
    data->write_finish_flag = 1;
    data->read_flag1 = 1;

    // reading the data from the shared memory region
    while (data->quit_flag == 0)
    {
        while (data->write_finish_flag == 1);
        printf("Reply : %s", data->user_string);
        data->read_flag1 = 1;
    }
    return NULL;
}

/******************** *writer_function(void *data1) ************************
 *  brief : Writer thread to write shared data
 *  param : data1 - shared data
 *  return: NULL
 ***************************************************************************/
void *writer_function(void *data1)
{
    struct shared_data *data = (struct shared_data *)data1;

    // copying the data into the shared memory region
    while (strcmp(data->user_string, "quit\n") != 0)
    {
        if (data->read_flag1 == 1)
        {
            data->write_finish_flag = 1;
            fgets(data->user_string, FOO, stdin);
            data->write_finish_flag = 0;
            while (data->read_flag1 == 0);
        }
    }
    data->quit_flag = 1;
    return NULL;
}
