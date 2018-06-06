#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>


#define BUFFER_SIZE   (10)

static uint8_t buffer[BUFFER_SIZE];

void * funcProducer() {
	int i = 0;
	int h = 0;
	while(1) {
		//sleep(2);
		if (i > 9) {
			i = 0;
		}
		buffer[i] = h;
		++i;
		++h;
	}
}
void * funcConsume() {
	int j = 0; 
	while(1) {
		//sleep(1);
		if (j > 9) {
			j = 0;	
		}
		printf("Buffer data on pos %d : %d", j, buffer[j]);
		++j;	
	}
}

int main() {
	pthread_t t0;
	pthread_t t1;
	
	pthread_create(&t0, NULL, funcProducer, NULL);
	pthread_create(&t1, NULL , funcConsume, NULL);
	pthread_join(t0, NULL);
	pthread_join(t1, NULL);
	return 0;
}
