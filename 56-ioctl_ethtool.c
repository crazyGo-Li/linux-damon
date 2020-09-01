#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <linux/sockios.h>
#include <linux/if.h>
#include <linux/ethtool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int ret = 0;
	int s;
	struct ifreq ifr;
	struct ethtool_cmd edata;
	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(s < 0)
	{
		fprintf(stderr, "%s(%d) : %s\n", __func__, __LINE__, strerror(errno));
		return -1;
	}

	strncpy(ifr.ifr_name, "eth0", sizeof(ifr.ifr_name));
	ifr.ifr_data = &edata;
	edata.cmd = ETHTOOL_GSET;
	ret = ioctl(s, SIOCETHTOOL, &ifr);
	if(ret < 0)
	{
		fprintf(stderr, "%s(%d) : %s\n", __func__, __LINE__, strerror(errno));
		close(s);
	}

	printf("speed : %d\n", ethtool_cmd_speed(&edata));
	
	return ret;
}
