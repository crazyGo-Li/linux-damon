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

/* �齨���͵İ� */
typedef struct icmp_packet{
	struct timeval tv_begin;
	struct timeval tv_end;
	short seq;
	int flag;
}pingm_packet;

/* �����͵İ��������������У�������Ϊ�ŵ������и��� */
static pingm_packet pingpacket[128];
static unsigned char send_buff[BUFFERSIZE];
static unsigned char recv_buff[2*K];
static struct sockaddr_in dest;
static int rawsock = 0;                                       //���ڽ������ݷ��ͺͽ��յ��׽���
static pid_t pid = 0;                                         //��pidΪ��־�ж��ǲ����Լ�������ping��
static int alive = 0;
static short packet_send = 0;                               
static short packet_recv = 0;
static struct timeval tv_begin, tv_end, tv_interval;
static char dest_str[80];



/*  icmp ���ݽṹ�����ڽ������Ͱ��������
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

	/* ���������Ƿ���ȷ */
	if(argc < 2)
	{
		icmp_usage();
		return -1;
	}

	/* ��ȡЭ���Ӧ�Ľṹ�� */
	protocol = getprotobyname(protoname);
	if(protocol == NULL)
	{
		ERR_MSG;
		return -2;
	}
	
	/* ����Ҫ�����ݿռ���г�ʼ������ */
	memcpy(dest_str, argv[1], strlen(argv[1]) + 1);
	memset(pingpacket, 0, sizeof(pingm_packet) * 128);

	/* ����ԭ���׽��� */
	rawsock = socket(AF_INET, SOCK_RAW, protocol->p_proto);
	if(rawsock < 0)
	{
		ERR_MSG;
		return -3;
	}

	/* ��ȡ����id��������֤�ǲ����Լ������İ� */
	pid = getpid();

	/* �����ջ������Ŵ󣬷�ֹ���صİ������ǵ� */
	setsockopt(rawsock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	
	/* ���������ַ���ߵ�ֵ�ipת����sockaddr_in�Ľṹ�� */
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	inaddr = inet_addr(argv[1]);
	if(inaddr == INADDR_NONE)
	{
		//���ת�����ɹ���ʱ��Ĭ��Ϊ�����url
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
		//ת���ɹ�ֱ�ӽ��Ѿ�ת���ĵ�ַ�������
		memcpy((char *)&dest.sin_addr, &inaddr, sizeof(inaddr));
	}

	/* ��ȷ��ʾ��Ϣ */
	inaddr = dest.sin_addr.s_addr;
	printf("PING %s (%d.%d.%d.%d) 56(84) bytes of data.\n",
		dest_str, 
		(inaddr&0x000000FF)>>0,
		(inaddr&0x0000FF00)>>8,
		(inaddr&0x00FF0000)>>16,
		(inaddr&0xFF000000)>>24);

	/* ����SINGINT�źţ�����ȡSINGINT��ʱ��ֱ���ͷ���Դ�����˳� */
	signal(SIGINT, icmp_sigint);

	/* �ֱ𴴽����ͺͽ����߳� */
	alive = 1;
	pthread_t send_id, recv_id;
	int err = 0;
	err = pthread_create(&send_id, NULL, icmp_send, NULL);          //���ڷ��͵��߳�
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


/* ��ӡping�Ľ�� */
static void icmp_statistics(void)
{
	unsigned long time = (tv_interval.tv_sec * 1000) + (tv_interval.tv_usec / 1000);
	printf("--- %s ping statics ---\n", dest_str);
	printf("%d packets transmitted, %d received, %d%cpacket loss, time %dms\n",
		packet_send, packet_recv, (packet_send - packet_recv)*100/packet_send, '%', time);
	return;
}

/* ����SINGINT�źŵ�ִ����ֹ��ʱ������˳����˳���ͨ��ȫ�ֱ���alive���в��� �� */
static void icmp_sigint(int signo)
{
	alive = 0;
	gettimeofday(&tv_end, NULL);
	tv_interval = icmp_tvsub(tv_end, tv_begin);
	return;
}

/* ���������е�Ԫ�غ͵�ǰ�յ��ı���бȶ� */
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
	int sum = 0;            //���յ�У����
	int odd = len & 0x01;   //�ж���ż

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

	/* ���ñ�ͷ */
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

/* ICMP����ͷ������ */
static int icmp_unpack(char *buf, int len)
{
	int i, iphdrlen;
	struct ip *ip = NULL;
	struct icmp *icmp = NULL;
	int rtt;

	//ԭ���׽����õ�����Ϣ��ip��װ���ģ���Ҫ�����ipͷ��Ȼ�����
	ip = (struct ip *)buf;
	iphdrlen = ip->ip_hl * 4;           //ipͷ�����ȣ������ֽ�����
	icmp = (struct icmp*)(buf + iphdrlen);
	len -= iphdrlen;                    //��ȡicmp�İ��ĳ���

	if(len < 8)
	{
		fprintf(stderr, "%s(%d) : icmp length is less than 8 \n", __func__, __LINE__);
		return -2;
	}

	//��icmp�İ����н������ж�pid�Ƿ�Ϊicmp����id�϶��ǲ����Լ�����
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

		/* ����������ã��������ڵ�ping�п�����us�ĵȼ��ģ������Ͳ��ܿ�����ȷ����Ϣ�� */
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

/* ʱ���ļ��� */
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


/* ���ͱ��� */
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

/* ���ձ��� */
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



