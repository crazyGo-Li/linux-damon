#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static int run = 1;
static int retvalue;

void *static_routine(void *arg)
{
	int *running = arg;
	printf("thread init success , arg: %d\n", *running);
	while(*running)
	{
		printf("thread is running\n");
		usleep(1);
	}
	printf("thread exit\n");
	retvalue = 8;
	pthread_exit((void *)&retvalue);
	return;
}

int main(void)
{
	pthread_t pt;
	int ret = -1;
	int times = 3;
	int i = 0;
	int *ret_join = NULL;
	ret = pthread_create(&pt, NULL, (void *)static_routine, &run);
	if(ret != 0)
	{
		printf("create thread fail\n");
		return 1;
	}
	usleep(1);
	for(;i<times;i++)
	{
		printf("master thread print\n");
		usleep(1);
	}
	run = 0;
	pthread_join(pt, (void *)&ret_join);

	printf("thread exit %d\n", *ret_join);
	return 0;
	return 0;
}
