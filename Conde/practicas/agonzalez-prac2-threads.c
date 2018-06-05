#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_LENGTH (5)

static uint8_t buffer[BUFFER_LENGTH];
static uint8_t cnt;

void * funcProducer()
{
	printf("enters func producer");
	while(1)
	{
		//sleep(2);
		buffer[cnt] = cnt+1;
		cnt = cnt+1;
	}
}

void * funcConsumer()
{
	printf("enters Func Consumer");
	while(1)
	{
		//sleep(1);
		if(cnt < BUFFER_LENGTH)
		{
			printf("enters comparison");
			printf("cnt=%d\t", cnt);
			printf("buffer =%d\t", buffer[cnt]);
		}
		else
		{
			cnt = 0;
		}
	}
}

int main ()
{
	cnt = 0;
	pthread_t t0, t1;
	pthread_create(&t0, NULL, funcProducer, NULL);
	pthread_create(&t1, NULL, funcConsumer, NULL);
	pthread_join(t0, NULL);
	pthread_join(t1, NULL);
	
	fprintf(stdin, "mi print\n");

	return 0;
}
