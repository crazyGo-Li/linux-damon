#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>

#define BUF_LEN 80

typedef int sem_t;

static char msg[] = "hello world";

sem_t CreateSem(key_t key, int value)
{
	
}

int main(void)
{
	key_t key;
	int semid,shmid;
	char i,*shms,*shmc;
	struct semid_ds buf;
	int value = 0;
	char buffer[BUF_LEN] = {0};
	pid_t p;

	//get key
	key = ftok("./", 1);
	if(-1 == key)
	{
		printf("get key fail : %s\n", strerror(errno));
		return key;
	}

	semid 
	
	return 0;
}
