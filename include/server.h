#ifndef SERVER_H
#define SERVER_H

#include "./networking.h"
#define BACKLOG 128

int create_server(struct socket_connection *server_socket, char *host, unsigned int port, unsigned int backlog);

#endif
// SERVER_H