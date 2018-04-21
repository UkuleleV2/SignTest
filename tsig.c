#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_PIDS 32
int main()
{
	printf("Test: %d\n",getpid());
	int bal[32];
	int c;
	volatile pid_t *pids;
	int NUM_CHILD = 2;
			
	pids = mmap(0, MAX_PIDS*sizeof(pid_t), PROT_READ|PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (!pids) 
	{
		printf("mmap failed");
		return 1;
	}	
	pid_t pid;
	memset((void *)pids, 0, MAX_PIDS*sizeof(pid_t));
	for (int i = 0; i <= NUM_CHILD; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			printf("Not correctly created procces, terminating other proccess\n");
		}
		else if (pid > 0)
		{
			pids[i]=pid;
		}
		
		sleep(1);
		printf("%d,ID: %d \n", i,getpid());
		
		
	}
	for (int i = 0; i <= NUM_CHILD; i++)
	{
	printf("dd: %d \n", pids[i]);
	}
	
	sleep(5);

	return 0;
}

/*

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PIDS 32

volatile pid_t *pids;

// Called for each child process
void do_child(void)
{
  int c;
  printf("Child: %d - pid array: ", getpid());

  for (c=0; c<10; c++) {
    printf("%d%s", pids[c], c==9?"\n":" ");
  }
}

int main(int argc, char **argv)
{
  int c;
  pid_t pid;

  // Map space for shared array
  pids = mmap(0, MAX_PIDS*sizeof(pid_t), PROT_READ|PROT_WRITE,
              MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (!pids) {
    perror("mmap failed");
    exit(1);
  }
  memset((void *)pids, 0, MAX_PIDS*sizeof(pid_t));

  // Fork children
  for (c=0; c<10; c++) {
    pid = fork();
    if (pid == 0) {
      // Child process
      do_child();
      exit(0);
    } else if (pid < 0) {
      perror("fork failed");
    } else {
      // Store in global array for children to see
      pids[c] = pid;
      sleep(1);
    }
  }
  exit(0);
}*/
