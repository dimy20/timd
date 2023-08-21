#include "conn.h"
#include "socket.h"
#include <asm-generic/errno.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

ReadBuffer::ReadBuffer(){
    size = 0;
    memset(data, 0, sizeof(data));
}

bool Conn::init(){ return socket_make_nonblocking(m_fd); };

/* Tries to fill the read buffer:
 * It will exit with true value only if read buffer is full or if we hit egain.
 * */
bool Conn::try_fill_read_buffer(){
    size_t n = MAX_MSG_SIZE;
    ssize_t ret;
    while(m_read_buffer.size < n){
        do{
            size_t available_bytes = n - m_read_buffer.size;
            ret = read(m_fd, m_read_buffer.data + m_read_buffer.size, available_bytes);
        }while(ret < 0 && errno == EINTR);

        if(ret == 0){
            fprintf(stderr, "EOF\n");
            return false;
        }else if(ret < 0){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                fprintf(stderr, "Data unavailable right now, try later\n");
                return true;
            }else{
                // An actual error
                perror("read");
                return false;
            }
        }

        assert(m_read_buffer.size + static_cast<size_t>(ret) < MAX_MSG_SIZE);
        m_read_buffer.size += static_cast<size_t>(ret);
    }

    // Buffer should be full at this point
    assert(m_read_buffer.size == MAX_MSG_SIZE);
    return true;
}
