#include <unistd.h>
#include <stdio.h>

int main() {
    // The list of args must end with a NULL
char* run_args[] = {"some random string", "-3", "5", "2045", NULL};
execvp("cal", run_args);    
    return 0;
}