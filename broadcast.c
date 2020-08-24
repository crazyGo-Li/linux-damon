#include "broadcast.h"

int HandleIPFound()
{
	int ret = -1;
	int sock = -1;
	struct sockaddr_in local_addr, from_addr;
	int form_len;
	int count = 1;
	fd_set readfd;
	char buff[BUF_MIDDLE_LEN];

	DEBUG_MSG("--->HandleIPFound");

	/* set time out limit */
	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	/* create socket */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(0 > sock)
	{
		ERROR_MSG;
		return -1;
	}

	/* initialization locak address */
	memset((void*)&local_addr, 0, sizeof(struct sockaddr_in));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(MCAST_PORT);

	/* bind local address to socket*/
	ret = bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr));
	if(-1 == ret)
	{
		ERROR_MSG;
		close(sock);
		return -2;
	}

	while(1)
	{
		/* initialization fd set */
		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);

		ret = select(sock+1, &readfd, NULL, NULL, &timeout);
		switch(ret)
		{
			case -1:
				break;
			case 0:
				break;
			default:
				if(FD_ISSET(sock, &readfd))
				{
					count = recvfrom(sock, buff, BUF_MIDDLE_LEN, 0, (struct sockaddr*)&from_addr, &form_len);
					DEBUG_MSG(buff);
					if(strstr(buff, IP_FOUND))
					{
						memcpy(buff, IP_FOUND_ACK, strlen(IP_FOUND_ACK)+1);
						count = sendto(sock, buff, strlen(buff), 0, (struct sockaddr *)&from_addr, form_len);
					}
				}
		}
	}
	
}

int IPFound()
{
	int i = 0;
	int ret = 0;
	int sock = -1;
	int times = 10;
	int so_broadcast = 1;
	struct ifreq ifr;
	struct sockaddr_in broadcast_addr;
	struct sockaddr_in from_addr;
	int from_len;
	int count = -1;
	fd_set readfd;
	char buff[BUF_MIDDLE_LEN];
	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(0  > sock)
	{
		ERROR_MSG;
		return -1;
	}

	/* set if name and get broadcast address */
	strncpy(ifr.ifr_name, IFNAME, strlen(IFNAME));
	if(ioctl(sock, SIOCGIFBRDADDR, &ifr) == -1)
	{
		ERROR_MSG;
		close(sock);
		return -1;
	}
	memcpy(&broadcast_addr, &ifr.ifr_broadaddr, sizeof(struct sockaddr_in));
	broadcast_addr.sin_port = htons(MCAST_PORT);

	/* set socket for broadcast */
	ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast));

	for(i=0; i<times; i++)
	{
		ret = sendto(sock, IP_FOUND, strlen(IP_FOUND), 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
		if(ret == -1)
		{
			continue;
		}
		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);
		ret = select(sock+1, &readfd, NULL, NULL, &timeout);
		switch(ret)
		{
			case -1:
				break;
			case 0:
				break;
			default:
				if(FD_ISSET(sock, &readfd))
				{
					count = recvfrom(sock, buff, BUF_MIDDLE_LEN, 0, (struct sockaddr*)&from_addr, &from_len);
					DEBUG_MSG(buff);
					if(strstr(buff, IP_FOUND_ACK))
					{
						printf("fount server, ip is %s", inet_ntoa(from_addr.sin_addr));
					}
					break;
				}
		}
	}

	
	
	return ret;
}
