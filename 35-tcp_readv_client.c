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

static int s;
struct iovec *vc=NULL;

void sig_process_client(int signo)
{
    printf("catch a exit signal\n");
    if(vc)
        free(vc);
    close(s);
    exit(0);
}

void sig_pipe(int signo)
{
    printf("socket is disconnect\n");
    if(vc)
        free(vc);
    close(s);
    exit(0);
}

void process_conn_client(s)
{
    char buffer[VBUFLEN] = {0};
    ssize_t size = 0;
    int i = 0;
    struct iovec *v = (struct iovec*)malloc(3*sizeof(struct iovec));
    if(v == NULL)
    {
        CODE_DEBUG;
    }
    vc = v;
    v[0].iov_base = buffer;
    v[1].iov_base = buffer + 10;
    v[2].iov_base = buffer + 20;

    v[0].iov_len = 10;
    v[1].iov_len = 10;
    v[2].iov_len = 10;
    for(;;)
    {
        size = read(0, v[0].iov_base, 10);
        if(size > 0)
        {
            v[0].iov_len = size;
            writev(s, v, 1);
            v[0].iov_len = 10;
            v[1].iov_len = 10;
            v[2].iov_len = 10;
            size = readv(s, v, 3);
            for(i = 0; i<3; i++)
            {
                if(v[i].iov_len > 0)
                {
                    write(1, v[i].iov_base, v[i].iov_len);
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    int err;

    if (argc == 1)
    {
        printf("please input server ip\n");
        return -1;
    }

    if(signal(SIGINT, sig_process_client) == SIG_ERR)
    {
        CODE_DEBUG;
        return -1;
    }

    if(signal(SIGPIPE, sig_pipe) == SIG_ERR)
    {
        CODE_DEBUG;
        return -1;
    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == -1)
    {
        CODE_DEBUG;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    connect(s, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    process_conn_client(s);
    close(s);

    return 0;
}

