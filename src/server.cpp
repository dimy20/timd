#include "server.h"
#include "socket.h"

#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include <unistd.h>

#define TODO(s) fprintf(stderr, "TODO: %s, at %s:%d\n", s, __FILE__, __LINE__); exit(1);

#define BACKLOG 32 // should i take this as arg?

static bool init_addr(sockaddr_in& addr, const char *host, uint16_t port){
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    if(inet_pton(AF_INET, host, &addr.sin_addr.s_addr) < 0){
        perror("inet_pton");
        return false;
    }
    return true;
};

bool Server::init(){
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(m_fd < 0){
        perror("socket");
        return false;
    }
    int v = 1;

    if(!socket_make_nonblocking(m_fd)){
        return false;
    }

    if(setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(int)) < 0){
        perror("setsockopt");
        return false;
    }

    struct sockaddr_in server_addr;

    if(!init_addr(server_addr, m_host.c_str(), m_port)){
        return false;
    }

    if(bind(m_fd, (struct sockaddr *)&server_addr, sizeof(sockaddr_in)) < 0){
        perror("bind");
        return false;
    }

    m_epoll_fd = epoll_create1(0);
    if(m_epoll_fd < 0){
        perror("epoll_create");
        return false;
    }

    struct epoll_event ev;
    ev.data.fd = m_fd;
    ev.events = EPOLLIN;

    if(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_fd, &ev) < 0){
        perror("epoll_ctl");
        return false;
    }

    return true;
}

bool Server::accept_new_connection(int client_fd){
    assert(client_fd > 0);
    printf("Received new connection, fd : %d\n", client_fd);


    auto conn = std::make_unique<Conn>(client_fd);
    if(!conn->init()){
        return false;
    }

    size_t n = m_connections.size();
    if(client_fd >= n){
        m_connections.resize(n + 1);
    }

    m_connections[static_cast<size_t>(client_fd)] = std::move(conn);
    return true;
};

bool Server::start_loop(){
    if(listen(m_fd, BACKLOG) < 0){
        perror("listen");
        return false;
    }
    printf("Server is now listening...\n");

    while(1){
        int client_fd;
        int ret;
        ret = epoll_wait(m_epoll_fd, m_events, MAX_EVENTS, -1);

        if(ret < 0){
            perror("epoll_wait");
            return false;
        }

        size_t num_events = static_cast<size_t>(ret);

        for(size_t i = 0; i < num_events; i++){
            if(m_events[i].events & EPOLLIN){
                if(m_events[i].data.fd == m_fd){
                    client_fd = accept(m_fd, NULL, NULL);
                    printf("New connection!\n");
                    //if(!accept_new_connection(client_fd)){
                    //    fprintf(stderr, "Failed to add connection");
                    //}
                }
            }

            if(m_events[i].events & EPOLLOUT){
                TODO("Implement write events!\n");
            }
        }

        /*
        for(auto& conn : m_connections){
            if(conn){
                if(conn->try_fill_read_buffer()){
                    if(errno != EAGAIN){
                        printf("Received data from client!\n");
                    }
                };
            }
        }
        */
    };
};



















