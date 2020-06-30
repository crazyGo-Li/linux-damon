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
#include <netinet/in.h>

#define PORT 9998
#define BUFLEN 1024
#define DEBUG_MSG printf("%s(%d): %s\n", __func__, __LINE__, strerror(errno))

void process_conn_clent(int s)
{
    char buffer[BUFLEN] = {0};
    ssize_t size = 0;

    size = read(0, buffer, BUFLEN);
    if(size > 0)
    {
        send(s, buffer, BUFLEN, 0);
        size = recv(s, buffer, BUFLEN, 0);
        write(1, buffer, size);
    }
    return;
}

int main(int argc, char * argv[])
{
    int ret = 0;
    int sc=-1, ss=-1;
    struct sockaddr_in server_addr;

    if(argc < 2)
    {
        printf("please input address\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    sc = socket(AF_INET, SOCK_STREAM, 0);
    if(sc == -1)
    {
        DEBUG_MSG;
        return -1;
    }

    ret = connect(sc, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    if(ret == -1)
    {
        DEBUG_MSG;
        close(sc);
    }
    process_conn_clent(sc);



}


