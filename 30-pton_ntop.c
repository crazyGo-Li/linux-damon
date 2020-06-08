#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define ADDRLEN 16

int main(int argc, char * argv[])
{
	struct in_addr ip;
	char IPSTR[]="192.168.1.1";
	char addr[ADDRLEN] = {};
	const char *str=NULL;
	int err = 0;
	err = inet_pton(AF_INET, IPSTR, (void *)&ip);
	if(err == -1)
	{
		printf("inet_pton fail %s\n", strerror(errno));
		return -1;
	}
	else
	{
		printf("inet_pton success string(%s) value(0x%x)\n", IPSTR, ip.s_addr);
	}

	ip.s_addr = htonl(192<<24|168<<16|12<<8|255);
	str = inet_ntop(AF_INET, (const void *)&ip, (char*)&addr[0], ADDRLEN);
	if(str)
	{
		printf("inet_ntop: ip, 0x%x is %s\n", ip.s_addr, str);
	}
	return 0;
}

