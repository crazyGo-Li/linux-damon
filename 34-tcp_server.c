#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

#define MAX 80
#define PORT 8080
#define SA struct socksddr

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for(;;)
    {
        bzero(buff, MAX);
        read(sockfd, buff, sizeof(buff));
        printf("from client: %s\t to Client:", buff);
        bzero(buff, MAX);
        n = 0;
        while((buff[n++] = getchar()) != '\n');
        write(sockfd, buff, sizeof(buff));
        if(strncmp("exit", buff, 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("Socket creation failed(%s)...\n", strerror(errno));
        exit(0);
    }
    else
    {
        printf("Socket successfully created...\n");
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
    {

    }

    return 0;
}

