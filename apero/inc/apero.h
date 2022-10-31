#ifndef _APERO_H_
#define _APERO_H_

#include <stdint.h>     /* types           */
#include <sys/types.h>  /* struct addrinfo */
#include <sys/socket.h> /* struct addrinfo */
#include <netdb.h>      /* struct addrinfo */

typedef struct {
    int fd;
    struct sockaddr_in saddr;
    char *dstdir;
    uint16_t dstport;
} cliente;

typedef struct {
    int fd;
    struct sockaddr_in s_addr;
    uint16_t port;
} servidor;



void print_addrinfo(struct addrinfo *);

int get_lo_socket(struct addrinfo **, char *);

void *get_in_addr(struct sockaddr *);

uint32_t crearCliente(char[INET_ADDRSTRLEN], uint16_t);

#endif /* _APERO_H_ */