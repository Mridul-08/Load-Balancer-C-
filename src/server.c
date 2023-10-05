#include "../include/server.h"

int create_server(
    struct socket_connection *server_socket, 
    char *host, 
    unsigned int port, 
    unsigned int backlog
){
    char ip[16];
    if(hostname_to_ip(host, port, ip) < 0){
        return -4;
    }

    if(get_socket(&(*server_socket), ip, port) < 0){
        return -1;
    }

    if(bind_to_socket(*server_socket) < 0){
        return -2;
    }

    if(listen_to_socket(*server_socket, backlog) < 0){
        return -3;
    }

    logger("[INFO] Server listening on http://%s:%s",
            server_socket->socket_name.host,
            server_socket->socket_name.port);
    return 0;
}