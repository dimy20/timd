#include <stdio.h>
#include "server.h"
#include <stdlib.h>

#define HOST "127.0.0.1"
#define PORT 8080

static Server server;
int main(){
    if(!sv_init(&server, HOST, PORT)){
        fprintf(stderr, "Failed to initialize server\n");
        exit(1);
    }

    sv_start_loop(&server);
    return 0;
}
