#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef int sem_t;

union semnu{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} arg;

sem_t CreateSem(key_y key, int values)
{
	union semnu sem;
	sem_t semid;
	sem.val = value;
	semid= semget(key, 0, IPC_CREAT|0666);
	if(semid == -1)
	{
		printf("create semaphoe fail : %s\n", strerror(errno));
		return -1;
	}

	semctl(semid, 0 SETVAL, sem);
	return semid;
}

int main(void)
{
	exit(0);
}
