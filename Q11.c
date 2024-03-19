#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#define BIG_NUM 50000000

int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "Program requires an integer argument.\n");
        exit(1);
    }

    long sum = 0;
    int argint = atoi(argv[1]);
    for (long i = 0; i < BIG_NUM; i++) {
        if (argint > 0) {
            sched_yield();
        }
        sum += i;
    }

}
