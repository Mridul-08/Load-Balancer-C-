#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#include "../include/networking.h"
#include "../include/server.h"
#include "../include/logging.h"



int main(int argc, char *argv[]){
    //start
    struct socket_connection client_socket;
    
    //create a listening socket
    if(create_server(&client_socket,NULL,NULL,NULL)){
        log_err("failed to create a server");
        return -1;
    }
    //make a connection loop to listen
    return 0;
}