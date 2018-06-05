#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
	pid_t pid;
	int i;

	printf("parent: %d\n", getpid());
	
	pid = fork();
	
	if (pid != 0)
	{
		printf("child: %d\n", getpid());
		for(i=0; i<5; ++i)
		{
			printf("%d \n", i);
		}
		exit(1);
	}
	
	printf("pid = %d\n", pid);
	
	while(1)
	{
		sleep(1);
	}
	return 0;
}
