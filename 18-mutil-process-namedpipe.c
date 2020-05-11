#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(void)
{
	int result = -1;
	int fd = -1;
	int nbytes = 0;
	int status = 0;
	pid_t pid = -1;
	result = mkfifo("named_fifo", 0666);
	char data_string[] = "123456789";
	char read_buffer[30] ="";
	

	if(result != 0)
	{
		printf("创建命名管道失败\n");
		return result;
	}

	pid = fork();
	if(pid < 0)
	{
		printf("创建多进程失败\n");
		return pid;
	}
	else if(pid == 0)
	{
		printf("子进程创建成功\n");
		fd = open("named_fifo", O_WRONLY);
		if(fd < 0)
		{
			printf("子进程开启管道失败");
			exit(-1);
		}
		nbytes = write(fd, data_string, strlen(data_string));
		if(nbytes >= 0)
		{
			printf("数据写入成功， 写入量: %d\n", nbytes);
			close(fd);
			exit(0);
		}
		else
		{
			printf("数据写入失败， 写入量: %d\n", nbytes);
			close(fd);
			exit(nbytes);

		}
	}
	else
	{
				
		fd = open("named_fifo", O_RDONLY);
		if(fd < 0)
		{
			printf("打开文件失败\n");
			exit(fd);
		}
		#if 1
		pid = waitpid(pid, &status, 0);
		if(pid < 0)
		{
			printf("等待子进程失败\n");
			return pid;
		}
		#endif

		nbytes = read(fd, read_buffer, sizeof(read_buffer));
		printf("*********************\n");
		printf("%s\n", read_buffer);
		printf("*********************\n");
		exit(0);
	}
	

	return 0;
}

