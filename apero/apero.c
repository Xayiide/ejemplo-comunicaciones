#include <stdint.h>     /* tipos           */
#include <sys/types.h>  /* struct addrinfo */
#include <sys/socket.h> /* struct addrinfo */
#include <netdb.h>      /* struct addrinfo */

#include <stdio.h>
#include <arpa/inet.h>  /* inet_ntop       */
#include <errno.h>      /* errno           */
#include <string.h>     /* strerror        */
#include <stdlib.h>     /* malloc          */
#include <unistd.h>     /* close           */

#include "inc/apero.h"

static int rellenarCliente(cliente *c) {
    struct sockaddr_in aux;
    int                tamdir;
    char dir[INET_ADDRSTRLEN];

    int ret;

    ret = getsockname(c->fd, (struct sockaddr *) &aux, (socklen_t *) &tamdir);

    if (ret != -1) {
        (void) inet_ntop(aux.sin_family, &(aux.sin_addr),
                         dir, INET_ADDRSTRLEN);
        c->dir_local.sin_addr.s_addr = aux.sin_addr.s_addr;
        c->dir_local.sin_port        = aux.sin_port;
        c->dirlocal    = dir;
        c->puertolocal = ntohs(aux.sin_port);
    }

    return ret;
}




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

cliente *crearCliente(char dir[INET_ADDRSTRLEN], uint16_t puerto) {
    cliente        *c;
    struct in_addr  ia_addr;
    int             ret;


    ret = inet_pton(AF_INET, dir, &ia_addr);
    if (ret != 1) {
        perror("inet_pton");
        return NULL;
    }

    c = malloc(sizeof(cliente));
    if (c == NULL) {
        perror("malloc");
        return NULL;
    }

    memset(&(c->saddr.sin_zero), 0, sizeof(c->saddr.sin_zero));
    c->saddr.sin_family = AF_INET;
    c->saddr.sin_port   = htons(puerto);
    c->saddr.sin_addr   = ia_addr;
    c->dirdst           = dir;
    c->puertodst        = puerto;
    
    /* Se rellenarán al llamar a connect() */
    c->dirlocal           = NULL;
    c->puertolocal        = 0;

    c->dir_local.sin_family      = AF_INET;
    c->dir_local.sin_addr.s_addr = INADDR_ANY;

    c->fd = socket(PF_INET, SOCK_STREAM, 0);
    if (c->fd == -1) {
        perror("socket");
        return NULL;
    }

    ret = bind(c->fd, (struct sockaddr *) &(c->dir_local),
               sizeof(struct sockaddr));
    if (ret == -1) {
        perror("bind");
        close(c->fd);
        return NULL;
    }

    return c;
}

servidor *crearServidor(char dir[INET_ADDRSTRLEN], uint16_t puerto) {
    servidor       *s;
    struct in_addr  ia_addr;
    int             ret;

    ret = inet_pton(AF_INET, dir, &ia_addr);
    if (ret != 1) {
        perror("inet_pton");
        return NULL;
    }

    s = malloc(sizeof(servidor));
    if (s == NULL) {
        perror("malloc");
        return NULL;
    }

    memset(&(s->saddr.sin_zero), 0, sizeof(s->saddr.sin_zero));
    s->saddr.sin_family = AF_INET;
    s->saddr.sin_port   = htons(puerto);
    s->saddr.sin_addr   = ia_addr;
    s->dir              = dir;
    s->puerto           = puerto;

    s->fd = socket(PF_INET, SOCK_STREAM, 0);
    if (s->fd == -1) {
        perror("socket");
        return NULL;
    }

    ret = bind(s->fd, (struct sockaddr *) &(s->saddr), sizeof(struct sockaddr));
    if (ret == -1) {
        perror("bind");
        close(s->fd);
        return NULL;
    }

    return s;
}

int eliminarCliente(cliente *c) {
    int ret;

    ret = close(c->fd);
    if (ret != 0) {
        perror("close");
    }
    else {
        free(c);
    }

    return ret;
}

int eliminarServidor(servidor *s) {
    int ret;

    ret = close(s->fd);
    if (ret != 0) {
        perror("close");
    }
    else {
        free(s);
    }

    return ret;
}

int conectarCliente(cliente *c) {
    int ret;

    ret = connect(c->fd, (struct sockaddr *) &(c->saddr),
                  sizeof(struct sockaddr));

    if (ret == -1) {
        perror("connect");
        close(c->fd);
    }
    else {
        ret = rellenarCliente(c);
        if (ret == -1) {
            perror("getsockname");
            close(c->fd);
        }
    }
    
    return ret;
}

void imprimirCliente(cliente *c) {
    if (c == NULL) {
        printf("Puntero nulo. Saliendo\n");
        return;
    }

    printf("Socket: %d\n", c->fd);
    printf("IP destino: %s\n", c->dirdst);
    printf("          = %d\n", c->saddr.sin_addr.s_addr);
    printf("Puerto destino: %d\n", c->puertodst);
    printf("              = %d\n", (ntohs(c->saddr.sin_port)));
    
    if (c->dirlocal != NULL && c->puertolocal != 0) {
        printf("IP local: %s\n", c->dirlocal);
        printf("        = %d\n", c->dir_local.sin_addr.s_addr);
        printf("Puerto local: %d\n", c->puertolocal);
        printf("            = %d\n", (ntohs(c->dir_local.sin_port)));
    }
}

void imprimirServidor(servidor *s) {
    if (s == NULL) {
        printf("Puntero nulo. Saliendo\n");
        return;
    }

    printf("Socket: %d\n", s->fd);
    printf("IP Escucha: %s\n", s->dir);
    printf("          = %d\n", s->saddr.sin_addr.s_addr);
    printf("Puerto escucha: %d\n", s->puerto);
    printf("              = %d\n", (ntohs(s->saddr.sin_port)));
}