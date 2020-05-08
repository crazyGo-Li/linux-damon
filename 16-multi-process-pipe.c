#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int result = -1;
	int fd[2], nbytes;
	pid_t pid;
	char string[] = "你好";
	char readbuffer[80] = {};

	int *write_fd = &fd[1];
	int *read_fd = &fd[0];

	result = pipe(fd);
	if(-1 == result)
	{
		printf("创建管道失败\n");
		return -1;
	}

	pid = fork();
	if(pid < 0)
	{
		printf("创建进程失败\n");
		return -1;
	}
	else if(pid == 0)
	{
		close(*read_fd);
		write(*write_fd, string, strlen(string));
		return 0;
	}
	else
	{
		close(*write_fd);
		nbytes = read(*read_fd, readbuffer, sizeof(readbuffer));
		printf("接收到%d个数据，内容位“%s“\n", nbytes, readbuffer);
	}
	
	return 0;
}
