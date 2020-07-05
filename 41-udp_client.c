#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define BUF_LEN 1024
#define PORT 9999
#define DEBUG_MSG printf("%s(%d) : %s\n", __func__, __LINE__, strerror(errno))


int main()
{
    int sc = -1;
    int len = sizeof(struct sockaddr);
    char buffer[BUF_LEN] = "hello world";
    ssize_t size = 0;
    struct sockaddr_in add_client;

    add_client.sin_family = AF_INET;
    add_client.sin_port = htons(PORT);
    add_client.sin_addr.s_addr = inet_addr("127.0.0.1");

    sc = socket(AF_INET, SOCK_DGRAM, 0);
    if(sc == -1)
    {
        DEBUG_MSG;
        return -1;
    }

    size = sendto(sc, buffer, BUF_LEN, 0, (struct sockaddr *)&add_client, len);

    if(size == -1)
        DEBUG_MSG;

    close(sc);


    return 0;
}

