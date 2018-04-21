#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
	pid_t NUMCHILD = fork();
	sleep(1);
	printf("Test \n");
	
	return 0;
}
