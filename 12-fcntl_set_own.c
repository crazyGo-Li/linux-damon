#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	int uid;
	int fd = open("test.txt", O_RDWR|O_CREAT);
	if(fd < 0)
	{
		printf("get test.txt fd fail\n");
		return -1;
	}

	uid = fcntl(fd, F_SETOWN, 10000);
	close(fd);
	
	return 0;
}
