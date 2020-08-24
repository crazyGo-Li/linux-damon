#include "broadcast.h"

int main(int argc, char *argv[])
{
	int ret = 1;
	int s;
	struct sockaddr_in mcast_addr;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == s)
	{
		perror("create socket fail");
		return -1;
	}

	memset(&mcast_addr, 0, sizeof(mcast_addr));

	mcast_addr.sin_family = AF_INET;
	mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
	mcast_addr.sin_port = htons(MCAST_PORT);

	while(1)
	{
		int n = sendto(s, MCAST_DATA, sizeof(MCAST_DATA), 0,(struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
		if(n < 0)
		{
			perror("sendto fail\n");
			close(s);
			return -2;
		}
		sleep(MCAST_INTERVAL);
	}
	return 0;
}
