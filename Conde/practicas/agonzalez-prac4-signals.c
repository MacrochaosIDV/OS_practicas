#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define EVENT_1 (1)
#define EVENT_2 (2)
#define EVENT_3 (3)

uint8_t flag;

void sig_handler(int signo)
{
	if(signo == SIGINT)
	{
		flag |= (1<<EVENT_1);
	}
	else if(signo == SIGUSR1)
	{
		flag |=(1<<EVENT_2);
	}
	else if(signo == SIGTERM)
	{
		flag |= (1<<EVENT_3);
		//printf("Terminating process");
	}
}

void miFunc()
{
	printf("flag = 0x%x\n", flag);
	flag = 0;
	exit(0);
}

int main(void)
{
	//uint8_t counter;
	if(signal(SIGINT, sig_handler)==SIG_ERR)
	{
		printf("\n Can't catch SIGINT");
	}
	else
	{
		printf("\nISR installed for SIGINT\n");
	}
	if(signal(SIGTERM, sig_handler)==SIG_ERR)
	{
		printf("\n Can't catch SIGTERM");
	}
	else
	{
		printf("\nISR installed for SIGTERM\n");
	}
	if(signal(SIGUSR1, sig_handler)==SIG_ERR)
	{
		printf("\n Can't catch SIGUSR1");
	}
	else
	{
		printf("\nISR installed for SIGUSR1\n");
	}
	flag = 0;
	while(1)
	{
		if(flag)
		{
			miFunc();
		}
		else
		{
			sleep(1);
		}
	}
}
