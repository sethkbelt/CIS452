/****************************************************************************
 * Title: Lab4pipe.c
 * Author: Seth Konynenbelt
 * Created: February 1, 2024
 * Description: This file explores forking and piping
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
#define MAX 1024

int main()
{
    // create forward and backward pipe variables
    int fd[2];
    int bd[2];
    size_t num, len;
    pid_t pid, child = 0;
    char str[MAX];
    int status = 0;

    // create forward and backward pipe
    if (pipe(fd) < 0)
    {
        perror("plumbing problem with fd");
        exit(1);
    }
    if (pipe(bd) < 0)
    {
        perror("plumbing problem with bd");
        exit(1);
    }
    printf("Pipe descriptors fd: read=%d write=%d\n", fd[0], fd[1]);
    printf("Pipe descriptors bd : read=%d write=%d\n", bd[0], bd[1]);

    // fork to create child
    if ((pid = fork()) < 0)
    {
        perror("fork failed");
        exit(1);
    }

    else if (pid == 0)
    { // Child
        close(fd[READ]);
        close(bd[WRITE]);

        int a_Counter_Child = 0;
        // point C
        printf("Type a sentence: \n");
        printf("Type quit to quit \n");

        do
        {
            // allow user to quit, write sentence to pipe, read 'a' count from parent
            fgets(str, MAX, stdin);
            printf("Sent by %d: %s", getpid(), str);
            len = strlen(str) + 1;
            write(fd[WRITE], &len, sizeof(len));
            write(fd[WRITE], (const void *)str, (size_t)len);
            if (strncmp(str, "quit", 4))
            {
                read(bd[READ], &a_Counter_Child, sizeof(a_Counter_Child));
                printf("Number of a's in sentance: %d\n", a_Counter_Child);
            }
        } while (strncmp(str, "quit", 4));

        // close after writing and reading
        close(fd[WRITE]); 
        close(bd[READ]);
        exit(0);
    }
    // parent
    close(fd[WRITE]);
    close(bd[READ]);

    int a_Counter = 0;
    do
    {
        // read from pipe to see what child sent
        read(fd[READ], &len, sizeof(len));
        num = read(fd[READ], (void *)str, len);
        a_Counter = 0;

        if (num != len)
        {
            perror("pipe read error\n");
            exit(1);
        }
        // count a's and allow print
        printf("Received by %d: %s", getpid(), str);
        if (strncmp(str, "quit", 4))
        {
            for (int i = 0; i < num; i++)
            {
                if (str[i] == 'a')
                    a_Counter++;
            }
        }
        write(bd[WRITE], &a_Counter, sizeof(a_Counter));

    } while (strncmp(str, "quit", 4));
    close(fd[READ]); // close after reading
    close(bd[WRITE]);

    child = wait(&status); // wait for child
    // printf("Child PID: %d (extra, just to verify) \n", child);
    return 0;
}
