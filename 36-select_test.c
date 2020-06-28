//
// Created by lizhixuan on 2020/6/28.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/types.h>

int main(void)
{
    fd_set rd;
    struct timeval tv;
    int err;

    FD_ZERO(&rd);
    FD_SET(0, &rd);

    tv.tv_sec = 5;
    tv.tv_usec = 0;
    err = select(1, &rd, NULL, NULL, &tv);

    if(err == -1)
    {
        perror("select()");
    }
    else if(err)
    {
        printf("Data is available now\n");
    }
    else
    {
        printf("no data within five seconds\n");
    }
    return 0;
}


