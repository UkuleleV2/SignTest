#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
#define MAX_PIDS 32
int main()
{


	int NUM_CHILD = 2;
	int pid_numbers [NUM_CHILD];			
	pid_t pid;
	
	for (int i = 0; i <= NUM_CHILD; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			printf("Not correctly created procces, terminating other proccess\n");
		}

		
	//	printf("%d,ID: %d \n", i,getpid());
	 	if((int) pid > 0)
	 	{
                pid_numbers[i] = (int) pid;
                printf("rodzic[%d]: Dziecka pid: %d\n", (int) getpid(),(int) pid);
      }		
		sleep(1);	
		
	}

	
	sleep(5);
	return 0;
}
