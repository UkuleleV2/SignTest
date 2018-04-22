#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define WITH_SIGNALS

#ifdef WITH_SIGNALS

int set_signal_parent = 0;

void parent_handler(int signum) {
    set_signal_parent = 1;
	printf("\nparent[%d]: Signal SIGINT caught.\n", getpid());

 }

 void child_handler(int signum) {
	printf("child[%d]: SIGTERM caught.\n", getpid());
}
#endif // WITH_SIGNAL



void child_function(){

    #ifdef WITH_SIGNALS
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, child_handler);
	#endif

    printf("child[%d]: I am child of %d process\n", (int) getpid(), (int) getppid());

    sleep(10);

    exit(0);
}


int main()
{
    pid_t pid;
    int child_number = 0;
    int child_executed = 0;
    int NUM_CHILD = 5;
    int pid_numbers [NUM_CHILD];
    int status;

    #ifdef WITH_SIGNALS
	for(int i = 0; i < NSIG; i++)
		signal(i, SIG_IGN);

	signal(SIGCHLD, SIG_DFL);

	signal(SIGINT, parent_handler);
	#endif
	int i;
    for(i=0;i<NUM_CHILD;i++){

           #ifdef WITH_SIGNALS
            if(set_signal_parent) {
                printf("\nparent[%d]:  Terminating signal was caught by parent. Creation of new childs will be abandoned.\n", getpid());
                for(int g=0; g<child_number;g++)
                    kill(pid_numbers[g], SIGTERM);
                break;
            }
            #endif

            pid = fork();

            printf("\n");

            if((int) pid != 0){
                pid_numbers[i] = (int) pid;
                printf("parent[%d]: New child has been created with pid: %d\n", (int) getpid(),(int) pid);
            }

            if(pid < 0){
                printf("child[%d]: fork() failed. Terminating all created child processess.", getpid());
                for(int g=0; g<child_number;g++)
                    kill(pid_numbers[g], SIGTERM);
                exit(1);
            }

            if(pid == 0)
               child_function();

        sleep(1);
        child_number++;
    }

		for (int z = 0; z<NUM_CHILD; z++)
	{
		printf("Well: %d \n", pid_numbers[z]);
	}


    printf("\n\nparent[%d]: Waiting for child proccesses to be executed\n\n");
	printf("%d, %d \n", i, child_number);
    while((pid = wait(&status)) != -1) {
        printf("parent[%d]: Child %d has been terminated.\n", (int) getpid(),(int) pid);
        child_executed++;
	}

    printf("\n\nparent[%d]: There are no more child processes. \n", (int) getpid());
    printf("parent[%d]: Thre has been %d childs executed\n", (int) getpid(),child_executed);

  /*  #ifdef WITH_SIGNALS
	for(int i = 0; i < NSIG; i++)
		signal(i, SIG_DFL);
	#endif*/

    return 0;
}

