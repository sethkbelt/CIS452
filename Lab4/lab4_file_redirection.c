/****************************************************************************
 * Title: file_redirection
 * Author: Seth Konynenbelt
 * Created: February 1, 2024
 * Description: This file explores file redirection using dup2()
 ***************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mach/error.h>

int main()
{
    char name[50];

    int fd = open("myoutput.txt",
                  O_WRONLY | O_CREAT | O_EXCL,
                  S_IRUSR | S_IWUSR);
    if (errno != 0)
    {
        perror("Unable to create myoutput.txt");
        return 0;
    }
    printf("Enter your name: ");
    fgets(name, 50, stdin);
    printf("You entered: %s\n", name);

    printf("Enter your neigbhor's name: ");
    fgets(name, 50, stdin);

    // then creates a fork
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        perror("fork failed");
        exit(1);
    }
    else if (pid != 0) // parent dup2
    {
        dup2(fd, fileno(stdout));
        printf("Parent printing");
    }
    // then child issues an exec system call
    else if (pid == 0)
    { // Child
        char *const argv[] = {"ls", "-l", NULL};
        if (execvp("ls", argv) < 0)
            printf("\n\n Exec did not work properly\n\n");

        exit(0);
    }

    close(fd);
    return 0;
}