#pragma once
#include <stdio.h>

#define MAX_MSG_SIZE 1024

struct ReadBuffer{
    ReadBuffer();
    char data[MAX_MSG_SIZE];
    size_t size;
};

struct Conn{
    Conn(int fd) : m_fd(fd) {};
    bool init();
    bool try_fill_read_buffer();
    int m_fd;
    ReadBuffer m_read_buffer;
};
