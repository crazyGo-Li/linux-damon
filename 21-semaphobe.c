#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>





#define BUF_LEN 24

typedef int sem_t;

union semnu{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} arg;

#if 0
struct sembuf{
	unsigned short sem_num;
	short sem_op;
	short sem_flag;
}
#endif

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

void SetValueSem(sem_t semid, int value)
{
	union semnu sem;
	sem.val = value;
	semctl(semid, 0, SETVAL, sem);
}

int GetValueSem(sem_t semid)
{
	union semnu sem;
	return semctl(semid, 0, GETVAL, sem);
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
	char path[BUF_LEN] = "./";
	int semid;
	char i;
	int value = 0;
	key = ftok(path, 'a');
	if(-1 == key)
	{
		printf("create key fail : %s\n", strerror(errno));
		return -1;
	}

	semid = CreateSem( key, 100);
	for(i = 0 ; i <= 3; i++)
	{
		Sem_P(semid);
		Sem_V(semid);
	}
	Sem_V(semid);
	value = GetValueSem(semid);

	printf("sem value is : %d\n", value);

	DestroySem(semid);

	return 0;
}
