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
void add_command_to_history(const char *command);
void print_history();
static const char *history[MYLIMIT];
static unsigned int history_count;

int main(int argc, char *argv[])
{
    history_count = 0;
    // user and system information for cmd line
    struct utsname unameSys;
    char *me;
    int unkown_commands = 0;

    // string tokenization
    // https://stackoverflow.com/questions/46105827/getrusage-on-child-process
    struct timeval child1_utime, childold_utime;
  char cwd[MYLIMIT];

    while (1)
    {
        // prompt user for command
        uname(&unameSys);
        me = getenv("USER");
        // printf("%s ", getcwd(cwd,MYLIMIT));
        printf("%s@%s %s ", me, unameSys.nodename, getcwd(cwd,MYLIMIT));

        // https://stackoverflow.com/questions/3919009/how-to-read-from-stdin-with-fgets
        char *line;
        int line_max = MYLIMIT;

        // line_max + 1 leaves room for the null byte added by fgets().
        line = malloc(line_max + 1);

        while (*(fgets(line, line_max + 1, stdin)) != '\n')
        {
            printf("%lu", strlen(line));
            if(strlen(line) == 2)
            {
                printf("bad\n");
                exit(101);
            }
            // when '\n' is hit, which is enter, then return
            if (line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';

                // if quit exit immediately, else break the loop
                if (strcmp(line, "quit") == 0)
                {
                    free(line);
                    printf("\nUser Quit\nYou had %d unkown commands\n", unkown_commands);
                    exit(101);
                }
                else
                {
                    break;
                }
            }
        }
        if(strcmp(line, "\n") == 0)
            line = "N/A";
            // exit(0);

        char *tok = strtok(line, " \n");

        // On the initial call to strtok(), its first arg is the input string
        // the delimiter is a SPACE or a NEWLINE
        char *command_tok[LINE_MAX];
        int i = 0;
        while (tok)
        {
            command_tok[i++] = tok;
            tok = strtok(NULL, " \n");
        }
        command_tok[i] = NULL;

        pid_t pid, child;
        int status;
        if ((pid = fork()) < 0)
        {
            perror("fork failure");
            exit(1);
        }

        else if (pid == 0)
        {
            if (execvp(command_tok[0], command_tok) < 0)
                exit(101);
        }

        else
        {
            int ret_child;
            struct rusage usage;
            struct rusage usage_old;

            int who_child = RUSAGE_CHILDREN;

            ret_child = getrusage(who_child, &usage_old);
            child = wait(&status);
            add_command_to_history(*command_tok);
            ret_child = getrusage(who_child, &usage);
            child1_utime = usage.ru_utime;
            childold_utime = usage_old.ru_utime;
            long long user_time = child1_utime.tv_usec - childold_utime.tv_usec;
            printf("\n\nTime:  %lld microseconds\n", user_time);
            printf("Involuntary Switches:  %ld\n", usage.ru_nivcsw - usage_old.ru_nivcsw);

            if (WEXITSTATUS(status) == 101)
                unkown_commands = unkown_commands + 1;
            if(strcmp(*command_tok, "history") == 0)
                print_history();
            if(strcmp(command_tok[0], "cd") == 0)
            {
                chdir(command_tok[1]);
            }
        }
    }
    return 0;
}

// https://stackoverflow.com/questions/5050479/history-implementation-in-a-simple-shell-program-in-c
void add_command_to_history(const char *command)
{
    if (history_count < MYLIMIT)
    {
        history[history_count] = strdup(command);
    }
    else
    {
        // free( history[0] );
        for (unsigned index = 1; index < MYLIMIT; index++)
        {
            history[index - 1] = history[index];
        }
        history[MYLIMIT - 1] = strdup(command);
    }
    history_count = history_count + 1;
}

void print_history()
{
    for (unsigned index = 0; index < history_count; index++)
    {
        printf("%d : %s\n", index, history[index]);
    }
}