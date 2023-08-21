#include <stdio.h>
#include "server.h"

#define HOST "127.0.0.1"
#define PORT 8080

int main(){
    Server server(HOST, PORT);

    if(!server.init()){
        fprintf(stderr, "Failed to initialize server\n");
        exit(1);
    }

    server.start_loop();

    return 0;
}
