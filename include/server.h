#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <sys/epoll.h>

#include "conn.h"

#define MAX_EVENTS 128

struct Server{
    Server(const char *host, uint16_t port) : 
        m_host(host), m_port(port) {};

    bool init();
    bool start_loop();
    bool accept_new_connection(int client_fd);

    int m_fd;
    int m_epoll_fd;
    std::string m_host;
    uint16_t m_port;
    std::vector<std::unique_ptr<Conn>> m_connections; // maps fd's to connections
    struct epoll_event m_events[MAX_EVENTS];
};
