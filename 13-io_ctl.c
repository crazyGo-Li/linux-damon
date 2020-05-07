#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/cdrom.h>

int main(void)
{
	int fd = open("/dev/cdrom", O_RDONLY|O_NONBLOCK);

	if(fd < 0)
	{
		printf("open cdrom fail\n");
		return -1;
	}

	if(!ioctl(fd, CDROMEJECT, NULL))
	{
		printf("����ʧ��\n");
	}
	else
	{
		printf("�����ɹ�\n");
	}
	close(fd);
	
	return 0;
}

