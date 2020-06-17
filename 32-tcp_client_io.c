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
#define BUFLEN 1024

/* init signal catch function */
typedef void (*sighandler_t)(int);
void sig_process(int);
void sig_pipe(int);

/* set socket as global variables */
static int s;

/* get message from stdin and sed to server */

void process_conn_client(int s)
{
	ssize_t size = 0;
	char buffer[BUFLEN] = {0};

	size = read(0, buffer, BUFLEN);
	if(size > 0)
	{
		send(s, buffer, size, 0);
		size = recv(s, buffer, 1024, 0);
		write(1, buffer, size);
	}
	return;
}

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
	err = connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	if(err == -1)
	{
		printf("connect fail: %s\n", strerror(errno));
		close(s);
		return -1;
	}
	process_conn_client(s);
	close(s);
	return 0;
}

void sig_process(int signo)
{
	printf("get SIGINT\n");
	close(s);
	exit(0);
}

void sig_pipe(int signo)
{
	printf("get SIGPIPE\n");
	return;
}
