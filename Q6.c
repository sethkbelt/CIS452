#include <stdio.h>
#include <sched.h>

int main(){
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
