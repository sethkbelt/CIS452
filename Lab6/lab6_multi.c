/****************************************************************************
 * Title: lab6_multi.c
 * Author: Seth Konynenbelt
 * Created: February 16, 2024
 * Description: This code exists as a writer and reader to shared memory
 *              using threading
 ***************************************************************************/

/* File include libraries */
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Pre-defined Macros */
#define FOO 4096

void *do_greeting3(void *arg);
void *writer_function(void *arg);
void *reader_function(void *arg);

// global (shared and specific) data
int sharedData = 5;
char val[FOO];
int flag[2] = {0, 0};
int quit_flag = 1;

int main()
{
    pthread_t thread1, thread2, thread3;
    void *result1, *result2, *result3;
    int status;

    // create and start two threads executing the "do_greeting3" function
    // pass each thread a pointer to its respective argument
    if ((status = pthread_create(&thread1, NULL, writer_function, &val[0])) != 0)
    {
        fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
        exit(1);
    }
    if ((status = pthread_create(&thread2, NULL, reader_function, &val[1])) != 0)
    {
        fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
        exit(1);
    }

    if ((status = pthread_create(&thread3, NULL, reader_function, &val[1])) != 0)
    {
        fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
        exit(1);
    }

    // join with the threads(wait for them to terminate
    //  get their return vals
    if ((status = pthread_join(thread1, &result1)) != 0)
    {
        fprintf(stderr, "join error %d:%s\n", status, strerror(status));
        exit(1);
    }
    if ((status = pthread_join(thread2, &result2)) != 0)
    {
        fprintf(stderr, "join error %d: %s\n", status, strerror(status));
        exit(1);
    }
    if ((status = pthread_join(thread3, &result3)) != 0)
    {
        fprintf(stderr, "join error %d: %s\n", status, strerror(status));
        exit(1);
    }

    return 0;
}

/************* create_forked_command(const char *command)******************
 *  brief : Create a forked process with the given command, execute it
 *  param : const char *input_line, the command to be added
 *  return: N/A
 *
 * //arguments:arg is an untyped pointer pointing to a character
// returns:a pointer to NULL
// side effects:prints a greeting
 ***************************************************************************/
void *reader_function(void *arg)
{

    while (quit_flag != 0)
    {
        if (flag[0] == 1)
        {
            printf("Reader: %s", val);
            flag[1] = 0;
        }
    }
    return NULL;
}

void *writer_function(void *arg)
{
    while (quit_flag != 0)
    {
        // fgets is a blocking function
        if (flag[1] == 0)
        {
            flag[0] = 0;
            fgets(val, FOO, stdin);
            flag[0] = 1;
            quit_flag = strcmp(val, "quit\n");
        }
    }
    return NULL;
}