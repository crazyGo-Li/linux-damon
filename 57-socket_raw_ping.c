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

static pingm_packet *icmp_findpacket(int seq);
static unsigned short icmp_cksum(unsigned char *data, int len);
static struct timeval icmp_tvsub(struct timeval end, struct timeval begin);
static void icmp_statistics(void);
static void icmp_pack(struct icmp *icmph, int seq, struct timeval *tv, int length);
static int icmp_unpack(char *buf, int len);
static void *icmp_recv(void *argv);
static void *icmp_send(void *argv);
static void icmp_sigint(int signo);
static vodi icmp_usage();

#define K 1024
#define BUFFERSIZE 71

static unsigned char send_buff[BUFFERSIZE];
static unsigned char recv_buff[2*K];
static struct sockaddr_in dest;
static in rawsock = 0;
static pid_t pid = 0;
static short packet_send = 0;
static short packet_recv = 0;



