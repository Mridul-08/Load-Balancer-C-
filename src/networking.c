#include "../include/networking.h"

int get_socket(
    struct socket_connection *server_socket, 
    char *address, 
    unsigned int port
){
    /**
        * @brief Creating a socket file descriptor

        Function Used: socket(family, type, protocol)

        family: AF_INET -> IPv4 and AF_INET6 -> IPv6
        type: SOCK_STREAM -> Stream Socket and SOCK_DGRAM -> Datagram Socket
        protocol: IPPROTO_TCP -> TCP Protocol and IPPROTO_UPD -> UDP Protocol

    **/

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        log_err("failed to intitate server");
        return -1;
    }

    /**
        * @brief Set the server socket options (is optional and helps in reuse of address and port)

        Function used: int setsockopt(int socket_fd, int level, int option_name, const void *option_value, socklen_t option_len)

        socket_fd: file descriptor of socket
        level: protocol level
        option_name: The socket option for which the value is to be set
        option_value: A pointer to the buffer in which the value for the requested option is specified.
        option_len: he size, in bytes, of the buffer pointed to by the option_value parameter.

        returns 0 on success and -1 on failure
    **/

    int option_value = 1;
    if(setsockopt(socket_fd,SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value))== -1){
        log_err("Failed to set server socket options");
        close(socket_fd);
        return -2;
    }

    /**
        * @brief setting and port for building the server address
        * @see https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
        * The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
        * inet_pton - convert IPv4 and IPv6 addresses from text to binary form
    */

    struct sockaddr_in server_address;
    memset(&server_address, '0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, address, &server_address.sin_addr);
    socklen_t server_address_size = sizeof(server_address);

    /**
     * @brief Get the details of the server
    **/

    char host_buffer[INET6_ADDRSTRLEN];
    char service_buffer[NI_MAXSERV];

    if (getnameinfo((struct sockaddr *)&server_address, server_address_size,host_buffer, sizeof(host_buffer), service_buffer,sizeof(service_buffer), NI_NUMERICSERV) != 0) {
        log_err("Failed to fetch information about the server");
        return -3;
    }

    server_socket->socket_fd = socket_fd;
    server_socket->address = server_address;
    strcpy(server_socket->socket_name.host, host_buffer);
    strcpy(server_socket->socket_name.port, service_buffer);

    return 0;
}
