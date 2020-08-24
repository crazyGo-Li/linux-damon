#ifndef _MULTICAST_H_
#define _MULTICAST_H_

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

#define MCAST_ADDR "224.0.0.88"
#define MCAST_PORT 8888
#define MCAST_DATA "BROADCAST TEST DATA"
#define MCAST_INTERVAL 5

#endif