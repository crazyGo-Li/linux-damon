//
// Created by lizhixuan on 2020/7/1.
//
#include <stdlib.h>

typedef struct Nic_
{
    char nic_name[64];
    char power[16];
    char mac[32];
    char active[16];
    char ip[32];
    char mask[32];
    char gateway[32];
    char ipv6[32][32];
    char ipv6mask[32];
    char ipv6gateway[32];
    int mtu;
    int speed;
};

