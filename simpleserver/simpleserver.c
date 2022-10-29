#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <unistd.h>      /* close     */
#include <arpa/inet.h>   /* inet_ntop */

#include "inc/simpleserver.h"
#include "../include/apero.h"

#define PORT   6969
#define PORT_S "6969"

int main(void) {
    struct addrinfo *res;
    int sockfd;

    int newfd;
    struct sockaddr_storage newaddr;
    socklen_t addr_size = sizeof(newaddr);
    char cip[INET6_ADDRSTRLEN];
    char buf[MAX_DATASIZE];
    int  numrecv;

    sockfd = get_lo_socket(&res, PORT_S);

    bind(sockfd, res->ai_addr, res->ai_addrlen);

    listen(sockfd, MAX_CONEXIONES);

    newfd = accept(sockfd, (struct sockaddr *)&newaddr, &addr_size);

    inet_ntop(newaddr.ss_family,
              get_in_addr((struct sockaddr *) &newaddr),
              cip, sizeof cip);
    
    printf("Conexión desde %s\n", cip);
    numrecv = recv(newfd, buf, MAX_DATASIZE -1, 0);
    if (numrecv == -1) {
        printf("Error recibiendo.\n");
        return -1;
    }
    
    buf[numrecv] = '\0';

    printf("Recibido: '%s'\n", buf);

    freeaddrinfo(res);
    close(sockfd);

    return 0;
}