//
// Created by lizhixuan on 2020/6/29.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 9999
#define DEBUG_MSG printf("%s(%d): %s\n", __func__, __LINE__, strerror(errno))

int main(int argc, char * argv[])
{
    int ret = 0;
    int sc=-1, ss=-1;
    struct sockaddr_in server_addr, client_addr;

    if(argc < 2)
    {
        printf("please input address\n");
        return -1;
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(argv[1]);
    client_addr.sin_port = htons(PORT);

    sc = socket(AF_INET, SOCK_STREAM, 0);
    if(sc == -1)
    {
        DEBUG_MSG;
        return -1;
    }




}


