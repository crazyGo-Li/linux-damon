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
	ifr.ifr_ifindex = 6;
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

	/* 获取MTU和MAC */
	err = ioctl(s, SIOCGIFMTU, &ifr);
	if(err != 0)
	{
		STDERR_DEBUG;
		close(s);
		return err;
	}
	else
	{
		printf("MTU: %d\n", ifr.ifr_mtu);
	}
	err = ioctl(s, SIOCGIFHWADDR, &ifr);
	if(err != 0)
	{
		STDERR_DEBUG;
		close(s);
		return err;
	}
	else
	{
		{
			char *hw =ifr.ifr_hwaddr.sa_data;
			int i = 0;
			printf("MACADDR: ");
			do{
				//这里的输出有问题，eth0的输出第二位成了FFFFFFC4
				printf("%02X%s", hw[i], (i < 5) ? ":" : "");
				i++;
			}while(i < 6);
			printf("\n");
		}
	}

	/* 获取发送队列的长度 */
	err = ioctl(s, SIOCGIFTXQLEN, &ifr);
	if(err < 0)
	{
		STDERR_DEBUG;
		close(s);
		return err;
	}
	else
	{
		printf("IFTXALEN: %d\n", ifr.ifr_qlen);
	}

	/* 获取ip相关的信息，ip，广播地址，目的地址等 */
	memcpy(ifr.ifr_name, "br0", sizeof(ifr.ifr_name));
	err = ioctl(s, SIOCGIFADDR, &ifr);
	if(err < 0)
	{
		STDERR_DEBUG;
		close(s);
		return err;
	}
	else
	{
		{
			struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
			char ip[16] = {0};
			if(inet_ntop(AF_INET, &sin->sin_addr.s_addr, ip, 16))
			{
				printf("IP: %s\n", ip);
			}
			else
			{
				STDERR_DEBUG;
			}
		}
	}
	err = ioctl(s, SIOCGIFBRDADDR, &ifr);
	if(err < 0)
	{
		STDERR_DEBUG;
		close(s);
		return err;
	}
	else
	{
		{
			struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
			char ip[16] = {0};
			if(inet_ntop(AF_INET, &sin->sin_addr.s_addr, ip, 16))
			{
				printf("SIOCGIFBRDADDR: %s\n", ip);
			}
			else
			{
				STDERR_DEBUG;
			}
		}
	}
	err = ioctl(s, SIOCGIFDSTADDR, &ifr);
	if(err < 0)
	{
		STDERR_DEBUG;
		close(s);
		return err;
	}
	else
	{
		{
			struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
			char ip[16] = {0};
			if(inet_ntop(AF_INET, &sin->sin_addr.s_addr, ip, 16))
			{
				printf("SIOCGIFDSTADDR: %s\n", ip);
			}
			else
			{
				STDERR_DEBUG;
			}
		}
	}
	err = ioctl(s, SIOCGIFNETMASK, &ifr);
	if(err < 0)
	{
		STDERR_DEBUG;
		close(s);
		return err;
	}
	else
	{
		{
			struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
			char ip[16] = {0};
			if(inet_ntop(AF_INET, &sin->sin_addr.s_addr, ip, 16))
			{
				printf("SIOCGIFNETMASK: %s\n", ip);
			}
			else
			{
				STDERR_DEBUG;
			}
		}
	}

	/* 修改网络IP地址 */
	{
		struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
		char ip[16];
		printf("set eth3 ip 192.168.0.25\n");
		memset(&ifr, 0, sizeof(ifr));
		memcpy(ifr.ifr_name, "eth3", sizeof(ifr.ifr_name));
		inet_pton(AF_INET, "192.168.0.25", &sin->sin_addr.s_addr);
		sin->sin_family = AF_INET;
		err = ioctl(s, SIOCSIFADDR, &ifr);
		if(err)
		{
			STDERR_DEBUG;
		}
		else
		{
			printf("set eth3 ip 192.168.0.25 success\n");
		}

		inet_pton(AF_INET, "255.255.255.0", &sin->sin_addr.s_addr);
		err = ioctl(s, SIOCSIFNETMASK, &ifr);
		if(err)
		{
			STDERR_DEBUG;
		}
		else
		{
			printf("set eth3 mask 255.255.255.0 success\n");
		}
	}

	close(s);
	return err;
}
