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
