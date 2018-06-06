#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main (int argc, char* argv[]) 
{
	pid_t pid;
	printf("Parent: %d\n", getpid());
	pid = fork();
	if(pid == 0)
	{
	
		printf("Child: %d\n ", getpid());
			
	}
	else {
	    int i = 0;
	    while(1){
		printf("Countdown to exit :  %d\n ", i);
		sleep(1);
	    }
	    exit(1);
	}
	//printf("", pid);
	//while(1)
	//{
	//	sleep(1);
	//}
	return 0;
}

