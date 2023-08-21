#pragma once

#include <stdint.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include "conn.h"

#define MAX_EVENTS 128
#define ADDR_STRING_MAX_SIZE 255

typedef struct{
    int fd;
    int epoll_fd;
    struct epoll_event events[MAX_EVENTS];
    struct sockaddr_in server_addr;
}Server;

bool sv_init(Server *server, const char *host, uint16_t port);
bool sv_start_loop(Server *server);
