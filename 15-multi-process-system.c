#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	int ret = 0;
	printf("当前进程号:%d\n", getpid());

	/*centos 7.6环境下需要右移8位获取最终的返回值，其他环境没有测试*/
	/*system中的系统调用最好使用命令的绝对路径*/
	ret = system("ping 10.192.44.254 -c 2") >> 8;

	printf("ret: %d\n", ret);
	return ret;
}
