#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#if 0
typedef void (*sighandler_t)(int);
void sig_handle(int signo)
{
	if(SIGSTOP == signo)
	{
		printf("get signal SIGSTOP\n");
	}
	else if(SIGKILL == signo)
	{
		printf("get signal SIGKILL\n");
	}
	else
	{
		printf("get signal %s\n", signo);
	}
	return;
}


int main(void)
{
	sighandler_t ret;
	ret = signal(SIGSTOP, sig_handle);
	if(SIG_ERR == ret)
	{
		printf("link SIGSTOP fail %s\n", strerror(errno));
		return -1;
	}
	ret = signal(SIGKILL, sig_handle);
	if(SIG_ERR == ret)
	{
		printf("link SIGKILL fail %s\n", strerror(errno));
		return -1;
	}

	for(;;);

	return 0;
}
#endif

typedef void (* signal_handler)(int);

void signal_handler_fun(int signal_num)
{
	printf("catch signal %d\n", signal_num);
}

int main(void)
{
	signal_handler p_signal = signal_handler_fun;
	signal(SIGINT, p_signal);
	for(;;);
	return 0;
}

