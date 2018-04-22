#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NUM_CHILD 3
#define WITH_SIGNALS
	
void Child()
{
	printf("Child[%d], My parent: %d \n", getpid(), getppid());
	sleep(10);
	exit(0);
}


static volatile sig_atomic_t doneflag = 0;
static void setdoneflag(int signo) 
{
	printf("Signal caught\n");
	doneflag = 1;
}


int main()
{	
	#ifdef WITH_SIGNALS
	for(int i = 0; i < NSIG; i++)
	{
		signal(i, SIG_IGN);
	}
	signal(SIGCHLD, SIG_DFL);
	signal(SIGINT, setdoneflag);
	#endif

	int arr[NUM_CHILD];
	int procc_executed = 0;
	pid_t pid;
	int status = 0;
	
	for (int i = 0; i < NUM_CHILD; i++)
	{

		#ifdef WITH_SIGNALS
		if(doneflag) 
		{
		    printf("Parent[%d]:  Signal has been caught. No more childs will be created.\n", getpid());
		    for(int g=0; g<i;g++)
		        kill(arr[g], SIGTERM);
		    break;
		}
		#endif
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

	#ifdef WITH_SIGNALS
	for(int i = 0; i < NSIG; i++)
	{
		signal(i, SIG_DFL);
	}
		
	#endif
	return 0;
}



