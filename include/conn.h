#pragma once
#include <stdio.h>
#include <stdbool.h>

#define MAX_MSG_SIZE 1024

typedef struct{
    char data[MAX_MSG_SIZE];
    size_t size;
}ReadBuffer;

typedef struct{
    int fd;
    ReadBuffer read_buffer;
}Conn;

bool conn_init(Conn *conn);
