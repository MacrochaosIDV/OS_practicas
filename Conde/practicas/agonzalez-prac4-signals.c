#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define EVENT_1 (1)
#define EVENT_2 (2)
#define EVENT_3 (3)

uint8_t flag;

void sig_handlr(int signo)
{
	if(signo == SIGINT)
	{
		//flag = (1<<EVENT_1);
	}
	if(signo == SIGTERM)
	{
		printf("Terminating process");
	}
}

int main(void)
{
	//uint8_t counter;
	if(signal(SIGTERM, sig_handlr)==SIG_ERR)
	{
		printf("\n Can't catch SIGTERM");
	}
	//flag = 0;
	while(1)
	{
		sleep(1);
	}
}
