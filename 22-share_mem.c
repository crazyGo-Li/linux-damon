#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/sem.h>


#define BUF_LEN 80

typedef int sem_t;
union semnu{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} arg;


static char msg[] = "hello world";

sem_t CreateSem(key_t key, int value)
{
	union semnu sem;
	sem_t semid;
	sem.val = value;
	semid= semget(key, 1, IPC_CREAT|0666);
	if(semid == -1)
	{
		printf("create semaphoe fail : %s\n", strerror(errno));
		return semid;
	}

	semctl(semid, 0, SETVAL, sem);
	return semid;

}

int Sem_P(sem_t sem_id)
{
	struct sembuf sops={0, +1, IPC_NOWAIT};
	return (semop(sem_id, &sops, 1));
}

int Sem_V(sem_t sem_id)
{
	struct sembuf sops={0, -1, IPC_NOWAIT};
	return (semop(sem_id, &sops, 1));
}

void DestroySem(sem_t semid)
{
	union semnu sem;
	sem.val = 0;
	semctl(semid, 0, IPC_RMID, sem);
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

	/*create share memeory*/
	shmid = shmget(key, 2014, IPC_CREAT|0604);

	/* create semaphobe */
	semid = CreateSem(key, 0);
	if(semid < 0)
	{
		return -1;
	}
	
	p = fork();
	if(p > 0)
	{
		shms = (char *)shmat(shmid, 0, 0);
		memcpy(shms, msg, strlen(msg) + 1);
		sleep(10);
		Sem_P(semid);
		shmdt(shmid);
	}
	else if(p == 0)
	{
		shmc = (char *)shmat(shmid, 0, 0);
		Sem_V(semid);
		printf("get msg: %s\n", shmc);
		shmdt(shmid);
	}
	else
	{
		printf("for fail: %s\n", strerror(errno));
		return -1;
	}
	
	return 0;
}
