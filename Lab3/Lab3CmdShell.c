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
#include <sys/resource.h>
#include <sys/wait.h>

#define LINEMAX 256
void add_command_to_history(const char *command);
void print_history();
void get_command(char *input_line);
void create_forked_command(char *input_line);

static const char *history[LINEMAX];
static unsigned int history_count = 0;
int unkown_commands = 0;

int main(int argc, char *argv[])
{
    // user and system information for cmd line
    struct utsname unameSys;
    char *me;

    // string to store current path
    char cwd[LINEMAX];

    while (1)
    {
        // prompt user for command and print prompt
        uname(&unameSys);
        me = getenv("USER");
        printf("%s@%s %s %% ", me, unameSys.nodename, getcwd(cwd, LINEMAX));

        // LINEMAX + 1 leaves room for the null byte added by fgets()
        char *input_line = malloc(LINEMAX + 1);
        get_command(input_line);

        // create forked process
        create_forked_command(input_line);
    }
    return 0;
}

/************* add_command_to_history(const char *command)******************
 *  brief : Add command to a global array history of commands
 *  param : const char *command, the command to be added
 *  return: N/A
 *  Note:  Help with this function from
 * https://stackoverflow.com/questions/5050479/history-implementation-in-a-simple-shell-program-in-c
 ***************************************************************************/
void add_command_to_history(const char *command)
{
    // dupliate the string and add it to history
    if (history_count < LINEMAX)
    {
        history[history_count] = strdup(command);
    }
    else
    {
        // free( history[0] );
        // go through array to add to history
        for (unsigned index = 1; index < LINEMAX; index++)
        {
            history[index - 1] = history[index];
        }
        history[LINEMAX - 1] = strdup(command);
    }
    history_count = history_count + 1;
}

/*************************** print_history() *******************************
 *  brief : Print the history of commands since terminal was open
 *  param : N/A
 *  return: N/A
 ***************************************************************************/
void print_history()
{
    for (unsigned index = 0; index < history_count; index++)
    {
        printf("%d : %s\n", index, history[index]);
    }
}

/************* add_command_to_history(const char *command)******************
 *  brief : Add command to a global array history of commands
 *  param : const char *command, the command to be added
 *  return: N/A
 *  Note:  Help with this function from
 * https://stackoverflow.com/questions/5050479/history-implementation-in-a-simple-shell-program-in-c
 ***************************************************************************/
void get_command(char *input_line)
{
    // get command from std input
    while (*(fgets(input_line, LINEMAX + 1, stdin)) != '\n')
    {
        // catch case when '\n' is hit, which is enter, then return
        if (input_line[strlen(input_line) - 1] == '\n')
        {
            // replace with \0 for formatting
            input_line[strlen(input_line) - 1] = '\0';

            // if quit exit immediately, else break the loop
            if (strcmp(input_line, "quit") == 0)
            {
                free(input_line);
                printf("\nUser Quit\nYou had %d unknown commands\n", unkown_commands);
                exit(101);
            }
            else
            {
                break;
            }
        }
    }
}

/************* create_forked_command(const char *command)******************
 *  brief : Create a forked process with the given command
 *  param : const char *input_line, the command to be added
 *  return: N/A
 ***************************************************************************/
void create_forked_command(char *input_line)
{
    // child usage statstics
    struct rusage usage;
    struct rusage usage_old;
    int who_child = RUSAGE_CHILDREN;
    struct timeval child_time, child_time_saved;
    pid_t pid;

    // if input is empty
    if (strcmp(input_line, "\n") == 0)
        input_line = "N/A";

    char *tok = strtok(input_line, " \n");

    // first strtok has delimiter space or newline
    char *tokenized_command[LINEMAX];
    int i = 0;
    while (tok)
    {
        tokenized_command[i++] = tok;
        tok = strtok(NULL, " \n");
    }
    tokenized_command[i] = NULL;

    // fork the process for command line
    int status;
    if ((pid = fork()) < 0)
    {
        perror("fork failure");
        exit(1);
    }

    // if you are the child, exec() to replace binary
    else if (pid == 0)
    {
        if (execvp(tokenized_command[0], tokenized_command) < 0)
            exit(101);
    }

    else
    {
        // get old child usage (getrusage gets all children)
        getrusage(who_child, &usage_old);
        wait(&status);
        
        // add command to history
        add_command_to_history(*tokenized_command);

        // get new child usage stats
        getrusage(who_child, &usage);
        child_time = usage.ru_utime;
        child_time_saved = usage_old.ru_utime;
        long long user_time = child_time.tv_usec - child_time_saved.tv_usec;
        
        printf("\n\nTime:  %lld microseconds\n", user_time);
        printf("Involuntary Switches:  %ld\n", usage.ru_nivcsw - usage_old.ru_nivcsw);

        // if unkown command, add to history
        if (WEXITSTATUS(status) == 101)
            unkown_commands = unkown_commands + 1;

        // special commands of "cd" or "history" for extra credit
        if (strcmp(*tokenized_command, "history") == 0)
            print_history();
        if (strcmp(tokenized_command[0], "cd") == 0)
        {
            chdir(tokenized_command[1]);
        }
    }
}
