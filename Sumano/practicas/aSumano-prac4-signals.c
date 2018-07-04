#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define EVENT_1 (1)
#define EVENT_2 (2)
#define EVENT_3 (3)

//uint8_t flagy;

void sig_handlr(int sig)
{
	if(sig == SIGUSR1)
	{
		printf("\n:: SIGUSR1 detected ::\n");
	}
	if(sig == SIGINT)
	{
		//flagy = (1 << EVENT_1);
		printf("Keyboard interruption detected\n");
		exit(0);
	}
	if(sig == SIGQUIT)
	{
		//flagy = (1 << EVENT_2);
		printf("Terminating process & freeing memory");
	}
}

int main()
{
	if(signal(SIGQUIT, sig_handlr) == SIG_ERR)
	{
		printf("\nCant catch SIGQUIT\n");
	}
	else
	{
		//
		printf("Stuff...");
	}
	if(signal(SIGINT, sig_handlr) == SIG_ERR)
	{
		//Error
	}
	else
	{
		printf("More stuff...\n");
	}
	if(signal(SIGUSR1, sig_handlr) == SIG_ERR)
	{

	}
	else
	{

	}
	//flagy = 0;

	while(1)
	{
		sleep(1);
	}
}
