#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	pid_t pid,ppid;
	pid = getpid();
	ppid = getppid();
	printf("pid: %d, ppid: %d\n", pid, ppid);
	pid = fork();
	if(pid < 0)
	{
		printf("�����ӽ���ʧ��(%d)\n", pid);
		return pid;
	}
	else if(pid == 0)
	{
		printf("����ֵ: %d, ���̺ţ� %d,�����̺ţ�%d\n", pid, getpid(), getppid());
	}
	else
	{
		printf("����ֵ: %d, ���̺ţ� %d,�����̺ţ�%d\n", pid, getpid(), getppid());
	}
	return 0;
}
