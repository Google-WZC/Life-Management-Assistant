#pragma once

#include <stdio.h>

#define MAX_ENTRIES 1000

struct data
{
    char id[5];     // 0001/0
    char date[9];   // 20250828/0
    char str[100];  // 买蔬菜花费20元
    char thing[70]; // 买蔬菜花费
    float money;    // 20
    char unit[10];  // 元
};

struct database
{
    struct data entries[MAX_ENTRIES];
    int count; // when exist data id==0000, count==1
    FILE *fp;  // connect to file
};
