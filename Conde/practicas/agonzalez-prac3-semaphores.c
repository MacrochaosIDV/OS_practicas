#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define NITER 100000

int cnt = 0, value;
sem_t sem;

void* Count(void* a)
{
	int i, tmp;
	sem_getvalue(&sem, &value);
	printf("\nValor del semaphore antes de wait: %d", value);
	sem_wait(&sem);
	for(i=0; i<NITER; i++)
	{
		tmp = cnt;
		tmp = tmp+1;
		cnt = tmp;
	}
	sem_getvalue(&sem, &value);
	printf("\nValor del semaphone antes de post: %d", value);
	sem_post(&sem);
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	sem_init(&sem, 0, 1);
	pthread_t tid1, tid2;
	if(pthread_create(&tid1, NULL, Count, NULL))
	{
		printf("\n ERROR creating thread 1");
		exit(1);
	}
	if (pthread_create(&tid2, NULL, Count, NULL))
	{
		printf("\n ERROR creating thread 2");
		exit(1);
	}

	if(pthread_join(tid1, NULL))
	{
		printf("\n ERROR joining thread 1");
		exit(1);
	}

	if(pthread_join(tid2, NULL))
	{
		printf("\n ERROR joining thread 2");
		exit(1);
	}

	if(cnt < 2*NITER)
		printf("\n BOOM! cnt is [%d], should be %d\n", cnt, 2*NITER);
	else
		printf("\n OK! cnt is [%d]\n", cnt);

        sem_getvalue(&sem, &value);
        printf("\nValor del semaphone antes de destroy: %d", value);

	sem_destroy(&sem);
}
