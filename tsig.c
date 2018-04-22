#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#define NUM_CHILD 3
void Child()
{
	printf("Child[%d], My parent: %d \n", getpid(), getppid());
	sleep(10);
	exit(0);
}

int main()
{	
	int procc_executed = 0;
	int arr[NUM_CHILD];
	pid_t pid;
	int status = 0;
	
	for (int i = 0; i < NUM_CHILD; i++)
	{
		pid = fork();
		if ( pid != 0)
		{
			arr[i] = pid;
		}
		if (pid < 0)
		{
			printf("Not correctly created procces, terminating other proccess\n");	
			for (int a = 0; a < i; a++)
			{
				printf("Termination PID: %d\n",arr[a]);	
				kill(arr[a],SIGTERM);
				exit(1);
			}
		}
		else if (pid == 0)
		{			
			Child();
		}
		sleep(1);
	}


	printf("Parent[%d]: Waiting for execution\n", getpid());
	while((pid = wait(&status)) > 0) 
	{
	        printf("Parent[%d]: Process %d has been terminated.\n", getpid(), pid);
	        procc_executed++;
	}
	printf("No more childs\n");
	printf("Number of proccess terminated: %d \n",procc_executed);

	return 0;
}



