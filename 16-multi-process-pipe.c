#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int result = -1;
	int fd[2], nbytes;
	pid_t pid;
	char string[] = "���";
	char readbuffer[80] = {};

	int *write_fd = &fd[1];
	int *read_fd = &fd[0];

	result = pipe(fd);
	if(-1 == result)
	{
		printf("�����ܵ�ʧ��\n");
		return -1;
	}

	pid = fork();
	if(pid < 0)
	{
		printf("��������ʧ��\n");
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
		printf("���յ�%d�����ݣ�����λ��%s��\n", nbytes, readbuffer);
	}
	
	return 0;
}
