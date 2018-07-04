#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_LENGTH (5)

static uint8_t buffer[BUFFER_LENGTH];
//static uint8_t cnt;
static pthread_mutex_t lock;
static uint8_t i;

void * funcProducer()
{
	uint8_t h = 0;
	//uint8_t *p = &buffer[0];
	while(1)
	{
		//sleep(2);
		pthread_mutex_lock(&lock);
		buffer[h % BUFFER_LENGTH] = h;
		printf("[Writing %d to pos %d]\n", h, h % BUFFER_LENGTH);
		++h;
		h = h & BUFFER_LENGTH;
		//cnt = 1;
		pthread_mutex_unlock(&lock);
	}
}

void * funcConsumer()
{
	while(1)
	{
		pthread_mutex_lock(&lock);
		printf("[Buffer data on %d : %d ]\n", i % BUFFER_LENGTH, buffer[i % BUFFER_LENGTH];
		
		pthread_mutex_unlock(&lock);
		//cnt = 0;
	}
}

int main ()
{
	//cnt = 0;
	pthread_t t0, t1;
	pthread_create(&t0, NULL, funcProducer, NULL);
	pthread_create(&t1, NULL, funcConsumer, NULL);
	
	int pthread_mutex_init(&lock, NULL);

	pthread_join(t0, NULL);
	pthread_join(t1, NULL);	

	pthread_mutex_destroy(&lock);
	fprintf(stdin, "mi print\n");

	return 0;
}
