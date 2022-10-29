#include <stdio.h>
#include <sys/types.h>  /* struct addrinfo */
#include <sys/socket.h> /* struct addrinfo */
#include <netdb.h>      /* struct addrinfo */

#include <arpa/inet.h>  /* inet_ntop       */
#include <errno.h>      /* errno           */
#include <string.h>     /* strerror        */

#include "inc/apero.h"

void print_addrinfo(struct addrinfo *ainfo) {
    struct addrinfo *p;
    void *addr;
    void *ipversion;
    char ipstr[INET6_ADDRSTRLEN];

    for (p = ainfo; p != NULL; p = p->ai_next) {
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipversion = "IPv4";
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipversion = "IPv6";
        }
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", (char *) ipversion, ipstr);
    }
}

int get_lo_socket(struct addrinfo **_ainfo, char *port) {
    int sockfd;
    struct addrinfo aiaux;
    struct addrinfo *ainfo = *_ainfo;
    memset(&aiaux, 0, sizeof(struct addrinfo));

    aiaux.ai_family   = AF_UNSPEC;
    aiaux.ai_socktype = SOCK_STREAM;
    aiaux.ai_flags    = AI_PASSIVE; /* localhost */

    getaddrinfo(NULL, port, &aiaux, &ainfo);

    sockfd = socket(ainfo->ai_family, ainfo->ai_socktype, ainfo->ai_protocol);

    if (sockfd < 0) {
        fprintf(stderr, "%s: %s\n", __PRETTY_FUNCTION__, strerror(errno));
    }
    
    return sockfd;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    } else {
        return &(((struct sockaddr_in6 *)sa)->sin6_addr);
    }
}