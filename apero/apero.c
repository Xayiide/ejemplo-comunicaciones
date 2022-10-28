#include <stdio.h>
#include <sys/types.h>  /* struct addrinfo */
#include <sys/socket.h> /* struct addrinfo */
#include <netdb.h>      /* struct addrinfo */

#include <arpa/inet.h>  /* inet_ntop       */

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

