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


/**
 * @brief Assign address to the socket
 * After the creation of the socket, the bind function binds the socket to the address and port number specified in addr(custom data structure).
**/

int bind_to_socket(struct socket_connection server_socket){

    /*
    * Function Used: int bind(int socket_fd, const struct sockaddr *addr,socklen_t addrlen);
    * bind() assigns the address specified by addr to the socket referred to by the file descriptor socket_fd.
    * addrlen specifies the size, in bytes, of the address structure pointed to by addr.
    */

    socklen_t server_address_size = sizeof(server_socket.address);
    if(bind( server_socket.socket_fd, (struct sockaddr *)&server_socket.address, server_address_size) < 0) {
        log_err("Failed to bind the address with the server socket");
        close(server_socket.socket_fd);
        return -1;
    }
   return 0;

}

/**
 * @brief Connect to the socket for data transfer, usually a client side function
 * connect() is used to connect to the socket to send and receive data. After a successful connection only, 
 * we can use other methods on the socket file descriptor to undergo various network operations
**/
int connect_to_socket(struct socket_connection server_socket){

    /**
       The connect() system call connects the socket referred to by the
       file descriptor sockfd to the address specified by addr.  The
       addrlen argument specifies the size of addr.  The format of the
       address in addr is determined by the address space of the socket
       sockfd.

       If the socket sockfd is of type SOCK_DGRAM, then addr is the
       address to which datagrams are sent by default, and the only
       address from which datagrams are received.  If the socket is of
       type SOCK_STREAM or SOCK_SEQPACKET, this call attempts to make a
       connection to the socket that is bound to the address specified
       by addr.
    **/
   socklen_t server_address_size = sizeof(server_socket.address);
    if(connect(server_socket.socket_fd,(struct sockaddr *)&server_socket.address, server_address_size) < 0){
        log_err("Failed to connect %s%s", server_socket.socket_name.host,server_socket.socket_name.port);
        close(server_socket.socket_fd);
        return -1;
    }
    return 0;
}
/**
 * @brief Listen for connections on the server socket
 * listen() marks the socket referred to by sockfd as a passive
 * socket, that is, as a socket that will be used to accept incoming
 * connection requests using accept(2).
**/
int listen_to_socket(struct socket_connection server_socket, unsigned int backlog){
    int listen_status  = listen(server_socket.socket_fd,backlog);
    if(listen_status < 0){
        log_err("Failed to listen on %s%s",server_socket.socket_name.host,server_socket.socket_name.port);
        close(server_socket.socket_fd);
        return -1;
    }
    return 0;
}

int hostname_to_ip(char *hostname, unsigned int port, char *ip){
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in *h;
    int rv;

    char port_str[7];
    sprintf(port_str,"%d",port);
    
    //cycle to all available IPs
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((rv = getaddrinfo(hostname, port_str, &hints, &servinfo))!=0){
        log_err("Cannot resolve address %s:%d",hostname, port);
        return -1;
    }

    //loop through all the results
    for (p = servinfo; p!= NULL; p = p->ai_next){
        h = (struct sockaddr_in *)p->ai_addr;
        strcpy(ip, inet_ntoa(h->sin_addr));
    }

    freeaddrinfo(servinfo); //cleanup
    return 0;
}
