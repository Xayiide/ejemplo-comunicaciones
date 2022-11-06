
#include "../include/apero.h"
#include "../include/comun.h"
#include "inc/simpleserver.h"

#include <stdio.h>    /* perror  */
#include <stdint.h>   /* uint8_t */
#include <unistd.h>   /* close   */
#include <inttypes.h> /* formato de print para los tipos de stdint.h */

uint8_t ejemplo_datos[6] = {
    0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x00
};


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
    char    datos_recb[MAX_DATASIZE];

    tam_x = sizeof(saddr_x);
    fd_x  = accept(s->fd, (struct sockaddr *) &saddr_x, &tam_x);
    c     = crearConexion(fd_x, saddr_x);

#ifdef DEBUG
    imprimirConexion(c);
#endif

    tam_recb = recv(c->fd, datos_recb, MAX_DATASIZE, 0);
    if (tam_recb == -1) {
        perror("recv");
        return 0;
    }

#ifdef DEBUG
    printColumns((uint8_t *) datos_recb, MAX_DATASIZE, 8);
#endif

    if (datos_recb[0] == 0x42) {
        letrasAleatorias(ejemplo_datos, MAX_DATASIZE);
        enviarDatos(c, ejemplo_datos, MAX_DATASIZE);
    }

    /* solo aceptamos un mensaje del cliente */
    eliminarConexion(c);

    return ret;
}

int enviarDatos(conexion *c, uint8_t *datos, uint16_t tam) {
    int     ret    = 0;
    ssize_t totenv = 0;
    ssize_t env    = 0;

    while (totenv != tam) {
        env = send(c->fd, datos, (size_t) tam, 0);
        if (env == -1) {
            perror("enviarDatos");
            ret = -1;
            break;
        }
        totenv += env;
    }

#ifdef DEBUG
    if (ret == 0) {
        printf("Todos los datos enviados correctamente ");
        printf("(%zd/%" PRIu16 ")\n", totenv, tam);
    }
#endif

    return ret;
}
