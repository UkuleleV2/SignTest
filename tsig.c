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


static volatile sig_atomic_t interrupt_flag = 0;
static void set_interrupt_flag(int signo) 
{
	printf("Parent[%d]: Signal caught\n", getpid());
	interrupt_flag = 1;
}


int main()
{	
	#ifdef WITH_SIGNALS
	for(int i = 0; i < NSIG; i++)
	{
		signal(i, SIG_IGN);
	}
	signal(SIGCHLD, SIG_DFL);
	signal(SIGINT, set_interrupt_flag);
	#endif

	int arr[NUM_CHILD];
	int procc_executed = 0;
	pid_t pid;
	int status = 0;
	
	for (int i = 0; i < NUM_CHILD; i++)
	{

		#ifdef WITH_SIGNALS
		if(interrupt_flag) 
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
			printf("Parent[%d]: Not correctly created procces, terminating other proccess\n",getpid());	
			for (int a = 0; a < i; a++)
			{
				printf("Parent[%d]: Termination PID: %d\n",getpid(),arr[a]);	
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
	printf("Parent[%d]: No more childs\n", getpid());
	printf("Parent[%d]: Number of proccess terminated: %d \n",getpid(),procc_executed);

	#ifdef WITH_SIGNALS
	for(int i = 0; i < NSIG; i++)
	{
		signal(i, SIG_DFL);
	}
		
	#endif
	return 0;
}



