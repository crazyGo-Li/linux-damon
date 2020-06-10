#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <stdlib.h>

#define PORT 8888
#define BACKLOG 2

void sig_process(int);

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

	bzero(&ss, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = PORT;

	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	if(err < 0)
	{
		printf("bind fail %s\n", strerror(errno));
		return -1;
	}
	for(;;)
	{
		int addrlen = sizeof(struct sockaddr);
		sc =accept(ss, (struct sockaddr&)*client_addr, sizeof(client_addr));
		if(sc < 0)
		{
			printf("accept fail \n", strerror(errno));
			continue;
		}

		pid = fork();
		if(pid == 0)
		{
			close(ss);
			process_conn_server(sc);
		}
		else
		{
			close(sc);
		}
	}
	
	return 0;
}

void sig_process(int signo)
{
	if(signo == SIG_PIP)
	{
		printf("get SIG_PIP\n");
		exit(0);
	}
	else if(signo == SIG_INT)
	{
		printf("get SIG_INT\n");
		exit(0);
	}
	else
	{
		return 0;
	}
}
