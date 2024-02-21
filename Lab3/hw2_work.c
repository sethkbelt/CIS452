#include <stdio.h>
#include <unistd.h>

int main()
{
int i, return_val;
for (i=0; i<4; i++)
return_val = fork();
printf("process started %d \n", return_val);
return 0;
}

// #include <sys/types.h>
// #include <stdio.h>
// #include <unistd.h>
// #include<sys/wait.h>

// int main()
// {
// pid_t pid, pid1;
// pid = fork();
// if (pid < 0) {
// fprintf(stderr, "fork() failed\n");
// return(1);
// }
// else if (pid == 0) {
// pid1 = getpid();
// printf("pid = %d\n", pid); // A
// printf("pid2 = %d\n", pid1); // B
// }
// else {
// pid1 = getpid();
// printf("pid = %d\n", pid); // C
// printf("pid2 = %d\n", pid1); // D
// wait (NULL);
// }
// return 0;
// }