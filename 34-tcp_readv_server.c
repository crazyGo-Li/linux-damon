//
// Created by lizhixuan on 2020/6/22.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#define CODE_DEBUG printf("%s(%d): %s\n", __func__, __LINE__, strerror(errno))
#define PORT 8080
#define BACKLOG 2
#define VBUFLEN 30

typedef void (*sighandler_t)(int signo);

static struct iovec *vs=NULL, *vc=NULL;
int ss=-1, sc=-1;

void sig_process(int signo)
{
    printf("catch a exit signal\n");
    _exit(0);
}

void sig_pipe(int signo)
{
    printf("catch a SIGPIPE signal\n");
    if(vs != NULL)
    {
        free(vs);
    }
    if(vc != NULL)
    {
        free(vc);
    }
    if(ss > 0) close(ss);
    if(sc > 0) close(sc);
    _exit(0);
}

void process_conn_server(int sc)
{
    char buffer[VBUFLEN] = {0};
    ssize_t size = 0;
    struct iovec *v = (struct iovec *)malloc(3*sizeof(struct iovec));
    if(v == NULL)
    {
        CODE_DEBUG;
        return;
    }

    vs = v;
    v[0].iov_base = buffer;
    v[1].iov_base = buffer+10;
    v[2].iov_base = buffer+20;

    v[0].iov_len = 10;
    v[1].iov_len = 10;
    v[2].iov_len = 10;

    for(;;)
    {
        size = readv(sc, v, 3);
        if(size == 0)
        {
            close(sc);
            return;
        }

        sprintf(v[0].iov_base, "%d", size);
        sprintf(v[1].iov_base, "bytes alt");
        sprintf(v[2].iov_base, "ogether\n");

        writev(sc, v, 3);
    }

}

int main(int argc, char* argv[])
{
    int err = 0;
    struct sockaddr_in server_addr, client_addr;
    pid_t pid;
    sighandler_t ret;

    /* catch signal */
    ret = signal(SIGINT, sig_process);
    if(ret == SIG_ERR)
    {
        CODE_DEBUG;
        return -1;
    }
    ret = signal(SIGPIPE, sig_pipe);
    if(ret == SIG_ERR)
    {
        CODE_DEBUG;
        return -1;
    }

    /* create socket file description */
    ss = socket(AF_INET, SOCK_STREAM, 0);
    if(ss < -1)
    {
        CODE_DEBUG;
        return -1;
    }

    /* set server`s address */
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    /* bind socket file description with server address */
    err  = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(err == -1)
    {
        CODE_DEBUG;
        close(ss);
        return -1;
    }

    /* set socket file description listen port */
    err = listen(ss, BACKLOG);
    if(-1 == err)
    {
        CODE_DEBUG;
        close(ss);
        return -1;
    }

    for(;;)
    {
        int addrlen = sizeof(struct sockaddr);
        sc =accept(ss, (struct sockaddr*)&client_addr, &addrlen);
        if(sc == -1)
        {
            CODE_DEBUG;
        }
        pid = fork();
        if(pid == 0)
        {
            printf("get connect from: %s", inet_ntoa(client_addr.sin_addr));
            process_conn_server(sc);
        }
        else
        {
            close(sc);
        }

    }
}


