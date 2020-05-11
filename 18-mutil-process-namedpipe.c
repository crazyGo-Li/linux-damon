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
		printf("���������ܵ�ʧ��\n");
		return result;
	}

	pid = fork();
	if(pid < 0)
	{
		printf("���������ʧ��\n");
		return pid;
	}
	else if(pid == 0)
	{
		printf("�ӽ��̴����ɹ�\n");
		fd = open("named_fifo", O_WRONLY);
		if(fd < 0)
		{
			printf("�ӽ��̿����ܵ�ʧ��");
			exit(-1);
		}
		nbytes = write(fd, data_string, strlen(data_string));
		if(nbytes >= 0)
		{
			printf("����д��ɹ��� д����: %d\n", nbytes);
			close(fd);
			exit(0);
		}
		else
		{
			printf("����д��ʧ�ܣ� д����: %d\n", nbytes);
			close(fd);
			exit(nbytes);

		}
	}
	else
	{
				
		fd = open("named_fifo", O_RDONLY);
		if(fd < 0)
		{
			printf("���ļ�ʧ��\n");
			exit(fd);
		}
		#if 1
		pid = waitpid(pid, &status, 0);
		if(pid < 0)
		{
			printf("�ȴ��ӽ���ʧ��\n");
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

