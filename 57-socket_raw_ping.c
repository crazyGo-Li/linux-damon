#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/types.h>

#define K 1024
#define BUFFERSIZE 72
#define ERR_MSG fprintf(stderr, "%s(%d) : %s", __func__, __LINE__, strerror(errno))

/* 组建发送的包 */
typedef struct icmp_packet{
	struct timeval tv_begin;
	struct timeval tv_end;
	short seq;
	int flag;
}pingm_packet;

/* 将发送的包都保存在数组中，个人认为放到链表中更好 */
static pingm_packet pingpacket[128];
static unsigned char send_buff[BUFFERSIZE];
static unsigned char recv_buff[2*K];
static struct sockaddr_in dest;
static int rawsock = 0;                                       //用于进行数据发送和接收的套接字
static pid_t pid = 0;                                         //以pid为标志判断是不是自己发出的ping包
static int alive = 0;
static short packet_send = 0;                               
static short packet_recv = 0;
static struct timeval tv_begin, tv_end, tv_interval;
static char dest_str[80];



/*  icmp 数据结构，用于进行填充和剥离分析的
struct icmp
{
	u_int8_t icmp_type;
	u_int8_t icmp_code;
	u_int16_t icmp_cksum;
	union
	{
		struct ih_idseq
		{
			u_int16_t icd_id;
			u_int16_t icd_seq;
		}icmp_hun;
	}
#define icmp_id icmp_hun.ih_idseq.icd_id
#define icmpseq icmp_hun.ih_ih_idseq.icd_seq
	union
	{
		u_int8_t id_data[1];
	}icmp_dun;
#define icmp_data icmp_dun.id_data
};
*/

static pingm_packet *icmp_findpacket(int seq);
static int icmp_cksum(unsigned char *data, int len);
static struct timeval icmp_tvsub(struct timeval end, struct timeval begin);
static void icmp_statistics(void);
static void icmp_pack(struct icmp *icmph, int seq, struct timeval *tv, int length);
static int icmp_unpack(char *buf, int len);
static void *icmp_recv(void *argv);
static void *icmp_send(void *argv);
static void icmp_sigint(int signo);
static void icmp_usage();

int main(int argc, char *argv[])
{
	struct hostent *host = NULL;
	struct protoent *protocol = NULL;
	char protoname[] = "icmp";
	unsigned long inaddr = 1;
	int size = 128 * K;

	/* 解析参数是否正确 */
	if(argc < 2)
	{
		icmp_usage();
		return -1;
	}

	/* 获取协议对应的结构体 */
	protocol = getprotobyname(protoname);
	if(protocol == NULL)
	{
		ERR_MSG;
		return -2;
	}
	
	/* 将需要的数据空间进行初始化操作 */
	memcpy(dest_str, argv[1], strlen(argv[1]) + 1);
	memset(pingpacket, 0, sizeof(pingm_packet) * 128);

	/* 创建原生套接字 */
	rawsock = socket(AF_INET, SOCK_RAW, protocol->p_proto);
	if(rawsock < 0)
	{
		ERR_MSG;
		return -3;
	}

	/* 获取进程id，用于验证是不是自己发出的包 */
	pid = getpid();

	/* 将接收缓冲区放大，防止返回的包被覆盖掉 */
	setsockopt(rawsock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	
	/* 将传入的网址或者点分的ip转换成sockaddr_in的结构体 */
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	inaddr = inet_addr(argv[1]);
	if(inaddr == INADDR_NONE)
	{
		//点分转换不成功的时候默认为输入的url
		host = gethostbyname(argv[1]);
		if(host == NULL)
		{
			ERR_MSG;
			close(rawsock);
			return -4;
		}
		memcpy((char *)&dest.sin_addr, host->h_addr, host->h_length);
	}
	else
	{
		//转换成功直接将已经转换的地址进行填充
		memcpy((char *)&dest.sin_addr, &inaddr, sizeof(inaddr));
	}

	/* 明确提示信息 */
	inaddr = dest.sin_addr.s_addr;
	printf("PING %s (%d.%d.%d.%d) 56(84) bytes of data.\n",
		dest_str, 
		(inaddr&0x000000FF)>>0,
		(inaddr&0x0000FF00)>>8,
		(inaddr&0x00FF0000)>>16,
		(inaddr&0xFF000000)>>24);

	/* 挂载SINGINT信号，当获取SINGINT的时候直接释放资源进行退出 */
	signal(SIGINT, icmp_sigint);

	/* 分别创建发送和接收线程 */
	alive = 1;
	pthread_t send_id, recv_id;
	int err = 0;
	err = pthread_create(&send_id, NULL, icmp_send, NULL);          //用于发送的线程
	if(err < 0)
	{
		ERR_MSG;
		close(rawsock);
		return -5;
	}
	err = pthread_create(&recv_id, NULL, icmp_recv, NULL);
	if(err < 0)
	{
		ERR_MSG;
		close(rawsock);
		return -5;
	}
	pthread_join(send_id, NULL);
	pthread_join(recv_id, NULL);
	close(rawsock);
	icmp_statistics();
	return 0;
}

static void icmp_usage()
{
	printf("ping www.baidu.com\n");
	return;
}


/* 打印ping的结果 */
static void icmp_statistics(void)
{
	unsigned long time = (tv_interval.tv_sec * 1000) + (tv_interval.tv_usec / 1000);
	printf("--- %s ping statics ---\n", dest_str);
	printf("%d packets transmitted, %d received, %d%cpacket loss, time %dms\n",
		packet_send, packet_recv, (packet_send - packet_recv)*100/packet_send, '%', time);
	return;
}

/* 挂载SINGINT信号当执行终止的时候调用退出，退出是通过全局变量alive进行操作 的 */
static void icmp_sigint(int signo)
{
	alive = 0;
	gettimeofday(&tv_end, NULL);
	tv_interval = icmp_tvsub(tv_end, tv_begin);
	return;
}

/* 遍历数组中的元素和当前收到的表进行比对 */
static pingm_packet *icmp_findpacket(int seq)
{
	int i = 0;
	pingm_packet *found = NULL;
	if(seq == -1)
	{
		for(i = 0; i < 128; i++)
		{
			if(pingpacket[i].flag == 0)
			{
				found = &pingpacket[i];
				break;
			}
		}
	}
	else if(seq >= 0 )
	{
		for(i = 0; i < 128; i++)
		{
			if(pingpacket[i].seq == seq)
			{
				found = &pingpacket[i];
				break;
			}
		}
	}
	return found;
}

static int icmp_cksum(unsigned char * data, int len)
{
	int sum = 0;            //最终的校验结果
	int odd = len & 0x01;   //判断奇偶

	while( len & 0xfffe) 
	{
		sum += *(unsigned short *)data;
		data += 2;
		len -= 2;
	}

	if(odd)
	{
		unsigned short tmp = ((*data) << 8) % 0xff00;
		sum += tmp;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	return ~sum;
	
}

static void icmp_pack(struct icmp * icmph, int seq, struct timeval * tv, int length)
{
	unsigned char i = 0;

	/* 设置报头 */
	icmph->icmp_type = ICMP_ECHO;
	icmph->icmp_code = 0;
	icmph->icmp_cksum = 0;
	icmph->icmp_seq = seq;
	icmph->icmp_id = pid & 0xffff;
	for(i = 0; i < length; i++)
	{
		icmph->icmp_data[i] = i;
	}

	icmph->icmp_cksum = icmp_cksum((unsigned char *)icmph, length);
	
	return;
}

/* ICMP报文头部剥离 */
static int icmp_unpack(char *buf, int len)
{
	int i, iphdrlen;
	struct ip *ip = NULL;
	struct icmp *icmp = NULL;
	int rtt;

	//原生套接字拿到的信息是ip封装过的，需要先提出ip头部然后解析
	ip = (struct ip *)buf;
	iphdrlen = ip->ip_hl * 4;           //ip头部长度，按照字节来的
	icmp = (struct icmp*)(buf + iphdrlen);
	len -= iphdrlen;                    //获取icmp的包的长度

	if(len < 8)
	{
		fprintf(stderr, "%s(%d) : icmp length is less than 8 \n", __func__, __LINE__);
		return -2;
	}

	//对icmp的包进行解析，判断pid是否为icmp——id断定是不是自己发的
	if((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == pid))
	{
		struct timeval tv_internel, tv_recv, tv_send;
		pingm_packet *packet = icmp_findpacket(icmp->icmp_seq);
		if(packet == NULL)
			return -1;
		packet->flag = 0;
		tv_send = packet->tv_begin;

		gettimeofday(&tv_recv, NULL);
		tv_internel = icmp_tvsub(tv_recv, tv_send);

		/* 这个样例不好，局域网内的ping有可能是us的等级的，这样就不能看到正确的信息了 */
		rtt = tv_internel.tv_sec * 1000 + tv_internel.tv_usec / 1000;

		printf("%d byte from %s: icmp_seq=%u ttl=%d time=%d ms\n",
			len,
			inet_ntoa(ip->ip_src),
			icmp->icmp_seq,
			ip->ip_ttl,
			rtt);
		packet_recv++;
		return 0;
	}
	else
	{
		return -3;
	}
	
}

/* 时间差的计算 */
static struct timeval icmp_tvsub(struct timeval end, struct timeval begin)
{
	struct timeval tv;
	tv.tv_sec = end.tv_sec - begin.tv_sec;
	tv.tv_usec = end.tv_usec - begin.tv_usec;

	if(tv.tv_usec < 0)
	{
		tv.tv_sec --;
		tv.tv_usec += 1000000;
	}
	return tv;
}


/* 发送报文 */
static void * icmp_send(void *argv)
{
	gettimeofday(&tv_begin, NULL);
	while(alive)
	{
		int size = 0;
		struct timeval tv;
		gettimeofday(&tv, NULL);
		pingm_packet *packet = icmp_findpacket(-1);
		if(packet)
		{
			packet->seq = packet_send;
			packet->flag = 1;
			gettimeofday(&packet->tv_begin, NULL);
		}
		icmp_pack((struct icmp *)send_buff, packet_send, &tv, 64);
		size = sendto(rawsock, send_buff, 64, 0, (struct sockaddr *)&dest, sizeof(dest));
		if(size < 0)
		{
			perror("sendto error");
			continue;
		}
		packet_send++;
		sleep(1);
	}
}

/* 接收报文 */
static void * icmp_recv(void * argv)
{
	struct timeval tv;
	tv.tv_usec = 200;
	tv.tv_sec = 0;
	fd_set readfd;
	while(alive)
	{
		int ret = 0;
		FD_ZERO(&readfd);
		FD_SET(rawsock, &readfd);
		ret = select(rawsock + 1, &readfd, NULL, NULL, &tv);

		switch (ret)
		{
			case -1:
				break;
			case 0:
				break;
			default:
				{
					int fromlen = 0;
					struct sockaddr from;
					int size = recv(rawsock, recv_buff, sizeof(recv_buff), 0);
					if(errno == EINTR)
					{
						perror("recvform error");
						continue;
					}
					ret = icmp_unpack(recv_buff, size);
					if(-1 == ret)
					{
						continue;
					}
				}
		}

	}
}



