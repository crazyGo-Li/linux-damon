#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/route.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>

#define ERROR_MSG fprintf(stderr, "%s(%d) : %s\n", __func__, __LINE__, strerror(errno))

int main(int argc, char *argv[])
{
	int ret = 0;
	int sockfd = -1;
	struct rtentry rt;
	struct sockaddr_in *sockinfo = (struct sockaddr_in *)&rt.rt_gateway;

	bzero(&rt, sizeof(rt));
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd ==  -1)
	{
		ERROR_MSG;
		return -1;
	}

	sockinfo->sin_family = AF_INET;
	sockinfo->sin_addr.s_addr  = inet_addr("10.254.254.1");

	sockinfo = (struct sockaddr_in *)&rt.rt_dst;
	sockinfo->sin_family = AF_INET;
	sockinfo->sin_addr.s_addr = INADDR_ANY;

	sockinfo = (struct sockaddr_in *)&rt.rt_genmask;
	sockinfo->sin_family = AF_INET;
	sockinfo->sin_addr.s_addr = INADDR_ANY;

	rt.rt_flags = RTF_UP | RTF_GATEWAY;
	rt.rt_metric  = 0;
	rt.rt_dev = "eth4";

	//设置路由的名称
	//memmove(rt.rt_dev, "eth0", sizeof(rt.rt_dev));

	//删除路由使用SIOCDELRT
	ret = ioctl(sockfd, SIOCADDRT, &rt);
	if(ret != 0)
	{
		printf("%d\n", errno);
		ERROR_MSG;
	}
	close(sockfd);
	
	return ret;
}
