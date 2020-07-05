#include "udp_lib.h"

void udpserv_echo(int s, struct sockaddr_in *client)
{
    ssize_t n;
    char buf[BUF_LEN] = {0};
    socklen_t len;
    while(1)
    {
        len = sizeof(struct sockaddr);
        n = recvfrom(s, buf, BUF_LEN, 0, (struct sockaddr *)client, &len);
        printf("get messages : %s\n", buf);
        sendto(s, buf, n, 0, (struct sockaddr *)client, len);
    }
    return;
}

void udpclie_echo(int s, struct sockaddr_in *to)
{
    char buf[BUF_LEN] = "this is client";
    struct sockaddr_in from;
    socklen_t len = sizeof(*to);
    sendto(s, buf, BUF_LEN, 0, (struct sockaddr*)to , len);
    recvfrom(s, buf, BUF_LEN, 0, (struct sockaddr *)&from, &len);
    printf("get message: %s\n", buf);
    return;
}
