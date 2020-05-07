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
	int accmode = -1;
	char buf[] = "FCNTL";
	int fd = open("test.txt", O_RDWR);
	if(0 > fd)
	{
		printf("open test.txt fail\n");
		return -1;
	}
	
	flags = fcntl(fd, F_GETFL, 0);
	accmode = flags & O_ACCMODE;
	if(accmode == O_RDONLY)
	{
		printf("STDIN READONLY\n");
	}
	else if(accmode == O_WRONLY)
	{
		printf("STDIN WRITEONLY\n");
	}
	else if(accmode == O_RDWR)
	{
		printf("STDIN RDWR\n");
	}
	else
	{
		printf("STDIN UNKNOWN MODE\n");
	}

	if(flags & O_APPEND)
	{
		printf("STDIN APPEND\n");
	}

	if(flags & O_NONBLOCK)
	{
		printf("STDIN NONBLOCK\n");
	}


	flags |= O_APPEND;

	accmode = flags & O_ACCMODE;
	if(accmode == O_RDONLY)
	{
		printf("STDIN READONLY\n");
	}
	else if(accmode == O_WRONLY)
	{
		printf("STDIN WRITEONLY\n");
	}
	else if(accmode == O_RDWR)
	{
		printf("STDIN RDWR\n");
	}
	else
	{
		printf("STDIN UNKNOWN MODE\n");
	}

	if(flags & O_APPEND)
	{
		printf("STDIN APPEND\n");
	}

	if(flags & O_NONBLOCK)
	{
		printf("STDIN NONBLOCK\n");
	}

	/*书中的代码是错误的，当设置结构体的时候需要添加结构指针，标志位的时候直接使用long int*/
	flags = fcntl(fd, F_SETFL, flags);
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

