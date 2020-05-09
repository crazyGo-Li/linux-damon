#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define K 1024
#define WRITELEN (128*K)

int main(void)
{
	int result = -1;
	pid_t pid = -1;
	int fd[2],nbytes;
	char string[] = "你好，管道";
	char readbuffer[10*K] = {};

	int *write_fd = &fd[1];
	int *read_fd = &fd[0];
	result= pipe(fd);

	if(result < 0)
	{
		printf("创建管道失败\n");
		return result;
	}

	pid = fork();
	if(pid < 0)
	{
		printf("创建子进程失败\n");
		return pid;
	}
	else if(pid == 0)
	{
		int write_size = WRITELEN;
		result = 0;
		close(*read_fd);
		while(write_size >= 0)
		{
			result = write(*write_fd, string, write_size);
			if(result > 0)
			{
				write_size -= result;	
				printf("写入数据： %d, 剩余数据：%d\n", result,write_size);
			}
			else
			{
				sleep(10);
			}
		}
		close(*write_fd);
	}
	else
	{
		close(*write_fd);
		sleep(5);
		while(1)
		{
			nbytes = read(*read_fd, readbuffer, sizeof(readbuffer));
			if(nbytes <= 0 )
			{
				printf("没有数据写入\n");
				break;
			}
			printf("接收到%d,内容为:%d\n", nbytes, readbuffer);
		}

		close(*read_fd);
	}
	
	
	return 0;
}
