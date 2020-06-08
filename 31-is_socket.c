#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int issockettype(int);

int main(int argc, char *argv[])
{
	int ret = issockettype(0);
	printf ("%d\n", ret);
	return 0;
}

int issockettype(int fd)
{
	struct stat st;
	int err = fstat(fd, &st);
	if(-1 == err)
	{
		printf("get file stat fail %s\n", strerror(errno));
		return -1;
	}

	if((st.st_mode & S_IFMT) == S_IFSOCK)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}
