#ifndef SERVER_H
#define SERVER_H

#include "./networking.h"
#define BACKLOG 128

int create_server(struct socket_connection *server_socket, char *host, unsigned int port, unsigned int backlog);
int accept_incoming_connection(struct socket_connection sever_socket);
#endif
// SERVER_H