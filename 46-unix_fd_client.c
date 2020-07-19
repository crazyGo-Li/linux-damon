#include "unix_fd.h"

void usage(char * s)
{
	printf("Usage: %s <unix_domain_file>\n", s);

}

int main(int argc, char *argv[])
{
	int fd;
	struct sockaddr_un sun;
	int ret = 0;
	fd_set rset;
	int maxfd;
	struct timeval tout;
	char buf[BUF_LEN];
	

	if(argc != 2)
	{
		usage((char *)argv[0]);
		return 0;
	}
	
	/* create client socket */
	fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(-1 == fd)
	{
		DEBUG_MSG;
		return -1;
	}

	/* fill server address */
	bzero(&sun, sizeof(sun));
	sun.sun_family = AF_LOCAL;
	strcpy(sun.sun_path, argv[1]);

	/* check socket file is exists or not */
	if(access(sun.sun_path, F_OK|W_OK) < 0)
	{
		DEBUG_MSG;
		close(fd);
		return -2;
	}

	/* connect server */
	ret = connect(fd, (struct sockaddr*)&sun, sizeof(sun));
	if(ret < 0)
	{
		DEBUG_MSG;
		close(fd);
		return -3;
	}

	while(1)
	{
		FD_ZERO(&rset);
		FD_SET(0, &rset);
		FD_SET(fd, &rset);
		maxfd = fd;
		tout.tv_sec = 5;
		tout.tv_usec = 0;

		select(maxfd+1, &rset, NULL, NULL, &tout);

		if(FD_ISSET(0, &rset))
		{
			bzero(buf, BUF_LEN);
			do
			{
				ret = read(0, buf, BUF_LEN-1);
			}
			while (ret < 0 && EINTR == errno);
			if(ret < 0)
			{
				DEBUG_MSG;
				continue;
			}
			if(ret == 0)
			{
				continue;
			}
			if(write(fd, buf, strlen(buf)) < 0)
			{
				perror("write () to socket\n");
				continue;
			}
			if(strncmp(buf, QUIT_STR, strlen(QUIT_STR)) == 0)
			{
				printf("client is existing\n");
				break;
			}
		}
		if(FD_ISSET(fd, &rset))
		{
			bzero(buf, BUF_LEN);
			do{
				ret = read(fd, buf, BUF_LEN-1);
			}while(ret < 0 && EINTR == errno);

			/* server is closed */
			if(ret == 0)
			{
				perror("get none messages from server\n");
				break;
			}
			printf("get messages from server : %s\n", buf);
			if(strncmp(buf, SERV_RESP_STR, strlen(SERV_RESP_STR)))
			{
				printf("server is existed\n");
				break;
			}
		}
	}

	close(fd);
	
	return 0;
}
