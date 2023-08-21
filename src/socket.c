#include <fcntl.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
// Enable non blocking io on a socket.
bool socket_make_nonblocking(int socket_fd){
    assert(socket_fd > 0);
    int socket_flags;
    socket_flags = fcntl(socket_fd, F_GETFL, 0);
    if(socket_flags < 0){
        perror("fcntl");
        return false;
    }
    socket_flags |= O_NONBLOCK;

    if(fcntl(socket_fd, F_SETFL, socket_flags) < 0){
        perror("fcntl");
        return false;
    }

    return true;
};
