#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>

#define PORT 8888

typedef void (*sighandler_t)(int);


void sig_process(int);
void sig_pipe(int);

/* set socket as global variables */
static int s;
void sig_process_client(int);

int main(int argc, char * argv[])
{
	int err = 0;
	struct sockaddr_in server_addr;
	sighandler_t ret;

	if(argc == 1)
	{
		printf("please input server addr\n");
		return -1;
	}

	/* attach signal int & pipe */
	ret = signal(SIGINT, sig_process);
	if(ret == SIG_ERR)
	{
		printf("attach signal int fail %s\n", strerror(errno));
		return -1;
	}
	ret - signal(SIGPIPE, sig_pipe);
	if(ret == SIG_ERR)
	{
		printf("attach signal pipe fail %s\n", strerror(errno));
		return -1;
	}

	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0)
	{
		printf("socket fail %s\n", strerror(errno));
		return -1;
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

	/* connect to server */
	connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	process_conn_client(s);
	
	return 0;
}

void sig_process(int signo)
{

}

void sig_pipe(int signo)
{

}

void sig_process_client(int signo)
{
	printf("get exists signo \n");
	close(s);
	exit(0);
}

