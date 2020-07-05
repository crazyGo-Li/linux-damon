#ifndef _UDP_LIB_H
#define _UDP_LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9999
#define BUF_LEN 1024
#define DEBUG_MSG printf("%s(%d) : %s\n", __func__, __LINE__, strerror(errno))


void udpserv_echo(int s, struct sockaddr_in *client);
void udpclie_echo(int s, struct sockaddr_in *to);

#endif

