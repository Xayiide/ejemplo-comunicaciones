#ifndef _APERO_H_
#define _APERO_H_

#include <stdint.h>     /* types           */
#include <sys/types.h>  /* struct addrinfo */
#include <sys/socket.h> /* struct addrinfo */
#include <netdb.h>      /* struct addrinfo */

typedef enum {
    CLIENTE  = 1,
    SERVIDOR = 2
} tipoCS;

typedef struct {
    int fd;
    struct sockaddr_in saddr;
    struct sockaddr_in dir_local;
    char     *dirdst;
    uint16_t  puertodst;
    char     *dirlocal;
    uint16_t  puertolocal;
} cliente;

typedef struct {
    int fd;
    struct sockaddr_in saddr;
    char *dir;
    uint16_t puerto;
} servidor;



void print_addrinfo(struct addrinfo *);

int get_lo_socket(struct addrinfo **, char *);

void *get_in_addr(struct sockaddr *);

cliente *crearCliente(char[INET_ADDRSTRLEN], uint16_t);

servidor *crearServidor(char[INET_ADDRSTRLEN], uint16_t);

int eliminarCliente(cliente *);

int eliminarServidor(servidor *);

int conectarCliente(cliente *);



void imprimirCliente(cliente *);

void imprimirServidor(servidor *);

#endif /* _APERO_H_ */