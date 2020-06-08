#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
	struct in_addr ip, local,network;
	char addr1[] = "192.168.1.1";
	char addr2[] = "255.255.255.255";
	char addr3[] = "192.16.1";
	char *str = NULL;
	char *str2 = NULL;
	int err = 0;
	
	err = inet_aton(addr1, &ip);
	if(err)
	{
		printf("inet_aton success, string : %s; value : 0x%x\n", addr1, ip.s_addr);
	}
	else
	{
		printf("inet_aton fail, string : %s\n", addr1, ip.s_addr);
	}

	ip.s_addr = inet_addr(addr1);
	if(ip.s_addr != -1)
	{
		printf("inet_addr success, string : %s; value : 0x%x\n", addr1, ip.s_addr);
	}
	else
	{
		printf("inet_addr fail, string : %s; value : 0x%x\n", addr1, ip.s_addr);
	}
	ip.s_addr = inet_addr(addr2);
	if(ip.s_addr != -1)
	{
		printf("inet_addr success, string : %s; value : 0x%x\n", addr2, ip.s_addr);
	}
	else
	{
		printf("inet_addr fail, string : %s; value : 0x%x\n", addr2, ip.s_addr);
	}

	/* convert  192.168.1.1 and 255.255.255.255 network to string */
	ip.s_addr = 192<<24|168<<16|1<<8|1;
	str = inet_ntoa(ip);
	ip.s_addr = 255<<24|255<<16|255<<8|255;
	str2 = inet_ntoa(ip);

	printf("inet_ntoa:\n	0x%x -> %s\n	%s\n", ip.s_addr, str, str2);

	ip.s_addr = inet_addr(addr3);
	if(ip.s_addr != -1)
	{
		printf("inet_addrL string is %s value is 0x%x\n", addr3, ip.s_addr);
	}
	else
	{
		printf("inet_addr: string %s convert fail: %s\n", addr3, strerror(errno));
	}
	str = inet_ntoa(ip);
	printf("inet_ntoa: string %s ,value 0x%x\n", str, ip.s_addr);

	inet_aton(addr1, &ip);
	local.s_addr = htonl(ip.s_addr);
	local.s_addr = inet_lnaof(ip);
	str = inet_ntoa(local);
	printf("inet_lnaof: %s, ip: 0x%x\n", str, local.s_addr);

	network.s_addr =  inet_netof(ip);
	printf("inet_netof: value 0x%x\n", network.s_addr);
	return 0;

}
