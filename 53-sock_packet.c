#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <string.h>

/* get mac address */
int get_mac_address(int fd)
{
	char ef[ETH_FRAME_LEN];
	int n;
	struct ethhdr *p_ethhdr;
	int i = 0;
	p_ethhdr = (struct ethhdr*)ef;
	

	n = read(fd, ef, ETH_FRAME_LEN);
	printf("dst MAC: ");
	for(i = 0; i< ETH_ALEN - 1; ++i)
	{
		printf("%02x-", p_ethhdr->h_dest[i]);
	}
	printf("%02x\n", p_ethhdr->h_dest[ETH_ALEN-1]);

	if(ntohs(p_ethhdr->h_proto) == 0x0800)
	{
		struct iphdr * p_iphdr = (struct iphdr*)(ef + ETH_HLEN);
		printf("src ip: %s\n", inet_ntoa(p_iphdr->saddr));
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	int err = -1;
	int fd = -1;
	char *ethname = "br0";
	struct ifreq ifr;
	/* 0003表示接收所有协议的包 */
	fd = socket(AF_INET, SOCK_PACKET, htons(0x0003));
	if(fd == -1)
	{
		perror("create socket fail\n");
		return -1;
	}

	/* get if`s flag */
	strcpy(ifr.ifr_name, ethname);
	err = ioctl(fd, SIOCGIFFLAGS, &ifr);
	if(err == -1)
	{
		perror("ioctl set SIOCGIFFLAG fail\n");
		close(fd);
		return err;
	}

	/* set if`s flag */
	ifr.ifr_flags |= IFF_PROMISC;
	err = ioctl(fd, SIOCSIFFLAGS, &ifr);
	if(err == -1)
	{
		perror("ioctl set SIOCSIFFLAG fail\n");
		close(fd);
		return err;
	}
	get_mac_address(fd);
	return 0;
	
}
