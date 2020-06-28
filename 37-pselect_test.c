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

int child_events = 0;
void child_sig_handler (int x)
{
    child_events++;
    signal(SIGCHLD, child_sig_hander);
}

int main(int argc, char * argv[])
{
    signal_t sigmask, orig_sigmask;
    return 0;
}
