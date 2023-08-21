#include "conn.h"
#include "socket.h"
#include <asm-generic/errno.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

static void init_read_buffer(ReadBuffer *buffer){
    assert(buffer != NULL);
    buffer->size = 0;
    memset(buffer->data, 0, sizeof(buffer->data));
};

bool conn_init(Conn *conn){
    assert(conn != NULL);
    init_read_buffer(&conn->read_buffer);
    return socket_make_nonblocking(conn->fd);
}
