#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>

#define FILELENGTH 80

int main(void)
{
	int flags = -1;
	char buf[] = "FCNTL";
	int fd = open("test.txt", O_RDWR|O_CREAT);
	if(0 > fd)
	{
		printf("open test.txt fail\n");
		return -1;
	}
	flags = fcntl(fd, F_GETFL, 0);

	flags |= O_APPEND;
	flags = fcntl(fd, F_SETFL, &flags);
	if(0 > flags)
	{
		printf("fail to set\n");
		close(fd);
		return -1;
	}

	write(fd, buf, strlen(buf));

	close(fd);
	return 0;
}

