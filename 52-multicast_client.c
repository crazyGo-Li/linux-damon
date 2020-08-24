#include "multicast.h"

int main(int argc, char *argv[])
{
	int s;
	struct sockaddr_in local_addr;
	int err = -1;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == s)
	{
		perror("socket");
		return -1;
	}

	memset(&local_addr, 0 ,sizeof(struct sockaddr_in));
	local_addr.sin_family = AF_INET;
	local_addr.sin_address.s_address = htonl(INADDR_ANY);
	local_addr.sin_port = htons(MCAST_PORT);

	err = bind(s, (struct sockaddr*)&local_addr, sizeof(struct sockaddr));
	if(-1 == err)
	{
		perror("bind");
		close(s);
		return -2;
	}

	int loop = 1;
	err = setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
	if(-1 == err)
	{
		perror("set socket options");
		return -3;
	}

	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	err = setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	if(-1 == err)
	{
		perror("member ship");
		close(s);
		return -4;
	}

	int times = 0;
	int addr_len = 0;
	char buff[BUFF_SIZE];
	int n = 0;
	for(times = 0; times < 5; ++times)
	{
		addr_len = sizeof(local_addr);
		memset(buff, 0, sizeof(buff));
		n = recvfrom(s, buff, BUFF_SIZE, 0, (struct sockaddr * )&local_addr, &addr_len);
		if(n == -1)
		{
			perror("recive from fail\n");
		}

		printf("recive &dst message from server: %s\n", times, buff);

		sleep(MCAST_INTERVAL);
	}

	err = setopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
	close(s);
	return 0;
	
	return 0;
}
