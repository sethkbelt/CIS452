/****************************************************************************
 * Title: Lab3CmdShell.c
 * Author: Seth Konynenbelt
 * Created: January 26, 2024
 * Description: This code exists as a small command shell to the user. The
 * function displays prompts, parses user input, and spawns a child process
 * to execute the command given
 ***************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <limits.h>

#define MYLIMIT 256

int main(int argc, char *argv[])
{
    // user and system information for cmd line
    struct utsname unameSys;
    char *me;
    // string tokenization
    char *tok;

    while (1)
    {
        // prompt user for command
        uname(&unameSys);
        me = getenv("USER");
        printf("%s@%s %s %% ", me, unameSys.nodename, getenv("PWD"));

        // https://stackoverflow.com/questions/3919009/how-to-read-from-stdin-with-fgets
        char *line;
        int line_max = MYLIMIT;

        // line_max + 1 leaves room for the null byte added by fgets().
        line = malloc(line_max + 1);

        while (*(fgets(line, line_max + 1, stdin)) != '\n')
        {
            printf("44 %s\n", line);
            // when '\n' is hit, which is enter, then return
            if (line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';
            printf("49 %s\n", line);

                // if quit exit immediately, else break the loop
                if (strcmp(line, "quit") == 0)
                {
                    free(line);
                    exit(0);
                }
                else
                {
                    break;
                }
            }
        }

        char *tok = strtok(line, " \n");

        // printf("Using strtok() on |%s|\n", line);
        // On the initial call to strtok(), its first arg is the input string
        // the delimiter is a SPACE or a NEWLINE
        char *command_tok[LINE_MAX];
        int i = 0;
        while (tok)
        {
            command_tok[i++] = tok;
            tok = strtok(NULL, " \n");
            printf("command: %s\n", command_tok[i]);
        }
        command_tok[i] = NULL;

        if(execvp(command_tok[0], command_tok) < 0)
            printf("Error executing command");
        // while (tok != NULL)
        // {
        //     printf("[%s]\n", tok);
        //     // On subsequent calls to strtok(), its first arg is NULL
        //     tok = strtok(NULL, " \n");
        // }

        // pid_t pid, child;
        // int status;
        // if ((pid = fork()) < 0)
        // {
        //     perror("fork failure");
        //     exit(1);
        // }
        // else if (pid == 0)
        // {
        //     printf("\nForked\n");
        //     // char *run_args[] = {tok, /*"-l", "-a",*/ NULL};
        //     char *command_tok[MYLIMIT];
        //     command_tok[0] = NULL;
        //     execvp(command_tok[0], command_tok);
        //     execvp(NULL, tok);
        //     // exec(line);
        //     exit(101);
        // }
        // else
        // {
        //     child = wait(&status);
        //     printf("\nChild PID %ld terminated with return status %d\n", (long)child, WEXITSTATUS(status));
        // }
        //       (b) the parent calls wait() to retrieve the child status

        //free(tok);

        printf("\nexit\n");
        return 0;
    }
}

// Properly handle error condition when the user enters an unknown command

// Find and use the appropriate system call to collect resource usage statistics about each executed process. Hint: use man -k usage to find the appropriate system call.

// output the "user CPU time used" for each individual child process spawned by the shell

// output the number of "involuntary context switches" experienced by each individual child process spawned by the shell

// repeat until the user advises to "quit"