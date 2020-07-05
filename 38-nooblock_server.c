//
// Created by lizhixuan on 2020/6/29.
//
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>

#define PORT 9998
#define BACKLOG 4
#define BUFLEN 1024

#define DEBUG_MSG printf("%s(%d): %s\n", __func__, __LINE__, strerror(errno))

int main(void)
{
    struct sockaddr_in server_addr, client_addr;
    int len = sizeof(struct sockaddr);
    int s_s = -1, s_c = -1;
    int ret = 0;
    char buffer[BUFLEN] = {0};
    ssize_t size = -1;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s_s = socket(AF_INET, SOCK_STREAM, 0);
    if(s_s == -1)
    {
        DEBUG_MSG;
        return -1;
    }

    ret = bind(s_s, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    if(ret == -1)
    {
        DEBUG_MSG;
        return -1;
    }

    /* get file description no block */
    ret = fcntl(s_s, F_SETFL, O_NONBLOCK);
    if(ret == -1)
    {
        DEBUG_MSG;
        return -1;
    }

    ret = listen(s_s, BACKLOG);
    if(ret == -1)
    {
        DEBUG_MSG;
        return -1;
    }

    for(;;)
    {
        while(s_c < 0)
        {
            s_c = accept(s_s, (struct sockaddr*)&client_addr, &len);
            if (s_c == -1)
            {
                DEBUG_MSG;
                sleep(1);
            }
        }

        do
        {
            size = recv(s_c, buffer, 1024, 0);
        }
        while(size < 0);

        if(strncmp(buffer, "HELLO", 5) == 0)
        {
            send(s_c, "OK", 3, 0);
            close(s_c);
            continue;
        }

        if(strncmp(buffer, "SHUTDOWN", 8) == 0)
        {
            send(s_c, "BYE", 4, 0);
            close(s_c);
            continue;
        }

    }

}


