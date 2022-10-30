#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>      /* memset    */
//#include <unistd.h>      /* close     */
#include <arpa/inet.h>   /* inet_ntop */

#include "inc/simpleserver.h"
#include "../include/comun.h"
#include "../include/apero.h"


int main(void) {
    struct addrinfo hints, *res;
    int sockfd;

    int newfd;
    struct    sockaddr_storage remote_ip;
    socklen_t remote_ip_len = sizeof remote_ip;
    char      remote_addr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    if (getaddrinfo(LISTENING_ADDR, LISTENING_PORT, &hints, &res) != 0) {
        printf("getaddrinfo error\n");
        return 1;
    }

    print_addrinfo(res);

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, MAX_CONEXIONES);


    newfd = accept(sockfd, (struct sockaddr *) &remote_ip, &remote_ip_len);
    inet_ntop(remote_ip.ss_family,
              get_in_addr((struct sockaddr *) &remote_ip),
              remote_addr,
              sizeof(remote_addr));

    printf("Connection from %s\n", remote_addr);

    freeaddrinfo(res);

    return 0;
}