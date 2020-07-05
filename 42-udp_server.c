#include "udp_lib.h"

int main()
{
    int s;
    int ret = 0;
    struct sockaddr_in addr_server, addr_client;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == s)
    {
        DEBUG_MSG;
        return -1;
    }

    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(PORT);
    addr_server.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(s, (struct sockaddr *)&addr_server, sizeof(addr_server));
    if(-1 == ret)
    {
        DEBUG_MSG;
        close(s);
        return -2;
    }

    udpserv_echo(s, &addr_client);

    return 0;
}

