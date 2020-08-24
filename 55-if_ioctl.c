#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <string.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define STDERR_DEBUG fprintf(stderr, "%s(%d): %s", __func__, __LINE__, strerror(errno))

int main(int argc, char *argv[])
{
	int s;
	int err = -1;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s < 0)
	{
		STDERR_DEBUG;
		return err;
	}

	/* 通过网络编号获取网卡的名称 */

	struct ifreq ifr;
	ifr.ifr_ifindex = 3;
	err = ioctl(s, SIOCGIFNAME, &ifr);
	if(err != 0)
	{
		STDERR_DEBUG;
		close(s);
		return err;
	}
	else
	{
		printf("nic number: %d\nnic name: %s\n", ifr.ifr_ifindex, ifr.ifr_name);
	}


	/* 获取网络接口的标志 */
	err = ioctl(s, SIOCGIFFLAGS, &ifr);
	if(err != 0)
	{
		STDERR_DEBUG;
		close(s);
		return err;
	}
	else
	{
		printf("flags: %d\n", ifr.ifr_flags);
	}

	

	close(s);
	return err;
}
