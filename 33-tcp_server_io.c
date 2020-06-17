#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>

#define PORT 8888
#define BACKLOG 2
#define BUFLEN 1024

void sig_process(int);

/* get client message, and send get size */
void process_conn_server(int s)
{
	ssize_t size = 0;
	char buffer[BUFLEN] = {};
	for(;;)
	{
		size = recv(s, buffer, BUFLEN, 0);
		if(size == 0)
		{
			return;
		}
		sprintf(buffer, "%d bytes altogether\n", size);
		send(s, buffer, strlen(buffer)+1, 0);
	}
}


int main(int argc, char *argv[])
{
	int ss, sc;
	struct sockaddr_in server_addr, client_addr;
	int err;
	pid_t pid;

	signal(SIGINT,sig_process);

	signal(SIGPIPE, sig_process);

	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0)
	{
		printf("create socket fail %s\n", strerror(errno));
		return  -1;
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = PORT;

	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err < 0)
	{
		printf("bind fail %s\n", strerror(errno));
		return -1;
	}

	err = listen(ss, BACKLOG);
	if(err == -1)
	{
		printf("listen fail %s\n", strerror(errno));
		return -1;
	}

	for(;;)
	{
		int addrlen = sizeof(struct sockaddr);
		sc =accept(ss, (struct sockaddr*)&client_addr, &addrlen);
		if(sc < 0)
		{
			printf("accept fail %s \n", strerror(errno));
			continue;
		}

		pid = fork();
		if(pid == 0)
		{
			printf("get connection\n");
			close(ss);
			process_conn_server(sc);
		}
		else
		{
			close(sc);
		}
	}
}

void sig_process(int signo)
{
	if(signo == SIGPIPE)
	{
		printf("get SIGPIPE\n");
		exit(0);
	}
	else if(signo == SIGINT)
	{
		printf("get SIGINT\n");
		exit(0);
	}
	else
	{
		return ;
	}
}
