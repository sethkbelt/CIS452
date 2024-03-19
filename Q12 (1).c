#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define NLOOPS 500000000
#define FORKLOOPS 50

int main(){
	int fail;
	struct sched_param param;
	sched_getparam(0, &param);
	time_t start, end;
	//printf("Priority number is: %d\n", param.sched_priority);
	
    pid_t pid;
   start = time(NULL); 
    for (int i = 0; i < FORKLOOPS; i++) {
	
        if ((pid = fork()) < 0){
            perror ("fork failed");
            exit (1);
        }
        if (pid == 0){//child

            //set mode to idle for this process
            if(i % 2){
                fail = sched_setscheduler(0,SCHED_IDLE, &param);
                if(fail == -1)
                {
                    perror("failed to change to RR policy");
                    return(-1);
                }
                //run the busy stuff and track time before and after busy work

                long sum = 0;
                for (long j = 0; j < NLOOPS; j++) {
                    sum += j;  // meaningless work to keep the CPU busy
                }
                end = time(NULL);
                double diff = difftime(end,start);
                printf("Total time taken using SCHED_IDLE Policy: %f seconds\n", diff);
		exit(2);
            }
            else{
                //clock_t start_time = clock();
                fail = sched_setscheduler(0,SCHED_OTHER, &param);
                long sum = 0;
                for (long j = 0; j < NLOOPS; j++) {
                    sum += j;  // meaningless work to keep the CPU busy
                }
              	end = time(NULL);
                double diff = difftime(end,start);
                printf("Total time taken using SCHED_OTHER Policy: %f seconds\n", diff);
	  	exit(2);
	    }
        }
    }
	for(int i = 0; i < FORKLOOPS; i++){
		wait(NULL);
	}

	return 0;
}
