#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

#include "server.h"
#include "socket.h"

#define TODO(s) fprintf(stderr, "TODO: %s, at %s:%d\n", s, __FILE__, __LINE__); exit(1);

#define BACKLOG 32 // should i take this as arg?

static bool init_addr(struct sockaddr_in *addr, const char *host, uint16_t port){
    assert(addr != NULL);
    addr->sin_port = htons(port);
    addr->sin_family = AF_INET;
    if(inet_pton(AF_INET, host, &addr->sin_addr.s_addr) < 0){
        perror("inet_pton");
        return false;
    }
    return true;
};


bool sv_init(Server *server, const char *host, uint16_t port){
    server->fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server->fd < 0){
        perror("socket");
        return false;
    }

    int v = 1;

    if(!socket_make_nonblocking(server->fd)){
        return false;
    }

    if(setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(int)) < 0){
        perror("setsockopt");
        return false;
    }

    if(!init_addr(&server->server_addr, host, port)){
        return false;
    }

    if(bind(server->fd,
            (struct sockaddr *)&server->server_addr,
             sizeof(server->server_addr)) < 0){
        perror("bind");
        return false;
    }

    if((server->epoll_fd = epoll_create1(0)) < 0){
        perror("epoll_create");
        return false;
    }

    struct epoll_event ev;
    ev.data.fd = server->fd;
    ev.events = EPOLLIN;

    if(epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, server->fd, &ev) < 0){
        perror("epoll_ctl");
        return false;
    }
    return true;
}

bool sv_start_loop(Server *server){
    if(listen(server->fd, BACKLOG) < 0){
        perror("listen");
        return false;
    }

    printf("Server is now listening...\n");

    while(1){
        int client_fd;
        int ret;
        ret = epoll_wait(server->epoll_fd, server->events, MAX_EVENTS, -1);

        if(ret < 0){
            perror("epoll_wait");
            return false;
        }

        size_t num_events = (size_t)ret;

        for(size_t i = 0; i < num_events; i++){
            if(server->events[i].events & EPOLLIN){
                int fd = server->events[i].data.fd;
                if(fd == server->fd){
                    client_fd = accept(server->fd, NULL, NULL);
                    printf("New connection!\n");
                }
            }

            if(server->events[i].events & EPOLLOUT){
                TODO("Implement write events!\n");
            }
        }
    };

};
