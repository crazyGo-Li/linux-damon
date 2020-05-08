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
		printf("创建子进程失败(%d)\n", pid);
		return pid;
	}
	else if(pid == 0)
	{
		printf("返回值: %d, 进程号： %d,父进程号：%d\n", pid, getpid(), getppid());
	}
	else
	{
		printf("返回值: %d, 进程号： %d,父进程号：%d\n", pid, getpid(), getppid());
	}
	return 0;
}
