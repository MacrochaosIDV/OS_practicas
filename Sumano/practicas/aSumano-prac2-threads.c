#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <semaphore.h>

#define BUFFER_SIZE   (10)

static uint8_t buffer[BUFFER_SIZE];
//static uint8_t write_OR_read;
static pthread_mutex_t flagy;
static uint8_t i;
sem_t sem;
int32_t sem_val;

void * funcProducer() {
	//uint8_t i = 0;
	uint8_t h = 0;
	//uint8_t * p = &buffer[0]; 
	while(1) 
	{
		//if(write_OR_read == 0) 
		{
		//sleep(2);
			//if (i > (BUFFER_SIZE-1)) 
			//{
			//	i = 0;
			//}
			//pthread_mutex_lock(&flagy);
			//buffer[h % BUFFER_SIZE] = h;
			sem_wait(&sem);
			buffer[i % BUFFER_SIZE] = h;
			//printf("[Writing %d to pos %d ]\n", h, i);
			printf("[Writing %d to pos %d ]\n", h, i % BUFFER_SIZE);
			sem_getvalue(&sem, &sem_val);
			printf("[Semaphore val: %d after wait. ]\n", sem_val);
			//++i;
			++h;
			//h = h % BUFFER_SIZE;
			//write_OR_read = 1;
			//pthread_mutex_unlock(&flagy);
			sem_post(&sem);
			printf("[Semaphore val: %d after post. ]\n", sem_val);
		}
	}
}
void * funcConsume() {
	//uint8_t j = 0;
	//uint8_t * p = &buffer[0]; 
	while(1) 
	{
		//if(write_OR_read == 1) 
		{
			//sleep(1);
			//if (j > (BUFFER_SIZE-1)) 
			//{
				//j = 0;	
			//}
			//pthread_mutex_lock(&flagy);
			sem_wait(&sem);
			printf("[Buffer data on pos %d : %d ]\n", i % BUFFER_SIZE, buffer[i % BUFFER_SIZE]);
			++i;
			i = i % BUFFER_SIZE;
			sem_getvalue(&sem, &sem_val);
			printf("[Semaphore val: %d after wait. ]\n", sem_val);
			//pthread_mutex_unlock(&flagy);
			sem_post(&sem);
			sem_getvalue(&sem, &sem_val);
			printf("[Semaphore val: %d after post. ]\n", sem_val);
			//write_OR_read = 0;
		}
	}
}

int main() {
	i = 0;
	sem_init(&sem, 0, 1);
	pthread_mutex_init(&flagy, NULL);
	//write_OR_read = 0;
	pthread_t t0;
	pthread_t t1;
	
	pthread_create(&t0, NULL, funcProducer, NULL);
	pthread_create(&t1, NULL , funcConsume, NULL);
	pthread_join(t0, NULL);
	pthread_join(t1, NULL);
	return 0;
}
