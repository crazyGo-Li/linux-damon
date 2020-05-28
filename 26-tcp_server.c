#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <errno.h>

#define PORT 8888
#define BACKLOG 2

int main(void)
{
	int ss, sc;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int err;
	pid_t pid;

	/* create socket */
	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0)
	{
		printf("create socket fail: %s\n", strerror(errno));
		return ss;
	}

	/* set server address */
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htol(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	/* bind socket */
	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err < 0)
	{
		printf("bind fail : %s\n", strerr(errno));
		close(ss);
		return -1;
	}

	/* set listen */
	err = listen(ss, BACKLOG);
	if(err < 0)
	{
		printf("listen fail %s\n", strerr(errno));
		close(ss);
		return -1;
	}

	for(;;)
	{
		socklen_t addrlen = sizeof(struct sockaddr);
		sc = accept(ss, (struct sockaddr *)&client_addr, &addrlen);
		if(sc < 0)
		{
			continue;
		}

		pid = fork();
		if(pid == 0)
		{
			close(ss);
		}
		else
		{
			close(sc);
		}
	}
	return 0;
}
