#ifndef _BROADCAST_H_
#define _BROADCAST_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <net/if.h>



#define IP_FOUND "IP_FOUND"
#define IP_FOUND_ACK "IP_FOUND_ACK"
#define MCAST_PORT 9999
#define BUF_MIDDLE_LEN 32
#define BUFF_SIZE 512
#define IFNAME "br0"
#define DEBUG_MSG(s) printf("%s(%d): %s\n", __func__, __LINE__, s)
#define ERROR_MSG printf("%s(%d): %s\n", __func__, __LINE__, strerror(errno))

int HandleIPFound();
int IPFount();

#endif

