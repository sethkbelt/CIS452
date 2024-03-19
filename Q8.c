#include <stdio.h>
#include <sched.h>
#include <errno.h>
#include <stdlib.h>

int main(){
	int fail;
	struct sched_param param;
	sched_getparam(0, &param);
	param.sched_priority = 25;
	printf("Priority number is: %d\n", param.sched_priority);
	fail = sched_setscheduler(0,SCHED_RR, &param);
	if(fail == -1)
	{
		perror("failed to change to RR policy");
		return(-1);
	}
	else
		printf("Somehow changed succesfully\n");



	int policyInUse = sched_getscheduler(0);
	if(policyInUse == SCHED_OTHER)
		printf("SCHED_OTHER in Use\n");
	else if(policyInUse == SCHED_FIFO)
		printf("SCHED_FIFO in Use\n");
	else if(policyInUse == SCHED_RR)
		printf("SCHED_RR in Use\n");
	else
		printf("Unknown policy\n");


	return 0;
}
