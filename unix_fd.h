#ifndef _UNIX_FD_H_
#define _UNIX_FD_H_
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define UNIX_DOMAIN "/tmp/unix.domain"
#define BACKLOG 5
#define QUIT_STR "quit"
#define SERV_RESP_STR "Server:"
#define BUF_LEN 1024
#define DEBUG_MSG printf("%s(%d): %s\n", __func__, __LINE__, strerror(errno))

#endif
