#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

/*SIGSTOP 和 SIGKILL 信号无法被捕获或忽略，signal会出错*/

#if 1
typedef void (*sighandler_t)(int);
void sig_handle(int signo)
{
	/*
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
	*/
	printf("%d\n", signo);
	return;
}


int main(void)
{
	sighandler_t ret, func=sig_handle;
	ret = signal(SIGINT, func);
	if(SIG_ERR == ret)
	{
		printf("link SIGINT fail %s\n", strerror(errno));
		return -1;
	}
	ret = signal(SIGTERM, sig_handle);
	if(SIG_ERR == ret)
	{
		printf("link SIGTERM fail %s\n", strerror(errno));
		return -1;
	}

	for(;;);

	return 0;
}
#endif

#if 0
typedef void (* signal_handler)(int);

void signal_handler_fun(int signal_num)
{
	printf("catch signal %d\n", signal_num);
}

int main(void)
{
	signal_handler p_signal = signal_handler_fun, ret;
	ret = signal(SIGKILL, p_signal);
	if(ret = SIG_ERR)
	{
		printf("error %s\n", strerror(errno));
		return -1;
	}
	for(;;);
	return 0;
}
#endif

