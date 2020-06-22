//
// Created by lizhixuan on 2020/6/22.
//
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/uio.h>

#define CODE_DEBUG printf("%s(%d): %s\n", __func__, __LINE__, strerror(errno))
#define PORT 8080
#define BACKLOG 2
#define VBUFLEN 30

int main(int argc, char *argv[])
{
    return 0;
}

