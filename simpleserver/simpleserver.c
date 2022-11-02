
#include "../include/apero.h"
#include "../include/comun.h"
#include "inc/simpleserver.h"

#include <stdio.h>  /* perror  */
#include <stdint.h> /* uint8_t */
#include <unistd.h> /* close   */


int iniciarServidor(servidor *s, uint8_t maxConexiones) {
    int ret;

    ret = listen(s->fd, maxConexiones);

    if (ret == -1) {
        perror("iniciarServidor");
    }

    return ret;
}

uint8_t bucleConexiones(servidor *s) {
    uint8_t ret = 1;   

    conexion *c;
    int                fd_x;
    struct sockaddr_in saddr_x;
    socklen_t          tam_x;

    ssize_t tam_recb;
    char datos_recb[MAX_DATASIZE];

    tam_x = sizeof(saddr_x);
    fd_x  = accept(s->fd, (struct sockaddr *) &saddr_x, &tam_x);
    c     = crearConexion(fd_x, saddr_x);
    imprimirConexion(c);

    tam_recb = recv(c->fd, datos_recb, MAX_DATASIZE, 0);
    if (tam_recb == -1) {
        perror("recv");
        return 0;
    }

    int i, cols;
    for (i = 0, cols = 0; i < MAX_DATASIZE; i++, cols++) {
        if (cols == 8) {
            cols = 0;
            printf("\n");
        }
        printf("%2.2X ", datos_recb[i]);
    }
    printf("\n");

    /* solo aceptamos un mensaje del cliente */
    eliminarConexion(c);

    if (datos_recb[0] == 0x43) {
        ret = 0;
    }

    return ret;
}