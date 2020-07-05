#include "udp_lib.h"


int main()
{
    int s;
    struct sockaddr_in addr_server;
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1==s)
    {
        DEBUG_MSG;
        return -1;
    }

    memset(&addr_server, 0, sizeof(addr_server));

    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_server.sin_port = htons(PORT);

    udpclie_echo(s, &addr_server);

    return 0;
}

