#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_LEN 1024
#define PORT 9999
#define DEBUG_MSG printf("%s(%d) : %s\n", __func__, __LINE__, strerror(errno))


int main()
{
    int ss = -1;
    int ret = 0;
    int len = sizeof(struct sockaddr);
    ssize_t size = 0;
    char buffer[BUF_LEN] = {0};
    struct sockaddr_in add_serv;
    struct sockaddr_in add_clie;

    add_serv.sin_family = AF_INET;
    add_serv.sin_addr.s_addr = htonl(INADDR_ANY);
    add_serv.sin_port = htons(PORT);

    ss = socket(AF_INET, SOCK_DGRAM, 0);
    if(ss == -1)
    {
        DEBUG_MSG;
        return -1;
    }

    ret = bind(ss, (struct sockaddr *)&add_serv, sizeof(struct sockaddr));
    if(ret == -1)
    {
        DEBUG_MSG;
        close(ss);
        return -2;
    }

    size = recvfrom(ss, buffer, BUF_LEN, 0, (struct sockaddr *)&add_clie, &len);
    if(-1 == size)
    {
        DEBUG_MSG;
        close(ss);
        return -2;
    }
    else
    {
        printf("get messages: %s\n", buffer);

    }

    close(ss);

    return 0;
}

