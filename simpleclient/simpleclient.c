
#include <stdint.h>

#include "../include/apero.h"
#include "../include/comun.h"
#include "inc/simpleclient.h"

#include <stdio.h>    /* perror */
#include <inttypes.h> /* formatos de print para los tipos de stdint.h */



int enviarDatos(cliente *c, uint8_t *datos, uint16_t tamdatos) {
    int     ret    = 0;
    ssize_t totenv = 0;
    ssize_t env    = 0;

    while (totenv != tamdatos) {
        env = send(c->fd, datos, (size_t) tamdatos, 0);
        if (env == -1) {
            perror("enviarDatos");
            ret = -1;
            break;
        }
        totenv += env;
    }
#ifdef DEBUG
    if (ret == 0) {
        printf("Todos los datos [%02X] enviados correctamente ", datos[0]);
        printf("(%zd/%" PRIu16 ")\n", totenv, tamdatos);
    }
#endif

    return ret;
}

int recibirDatos(cliente *c, uint8_t *datos, uint16_t tamdatos) {
    int tam_recb;

    tam_recb = (int) recv(c->fd, datos, tamdatos, 0);

    if (tam_recb > tamdatos) {
        printf("Se han recibido más datos de los que cabían en el buffer ");
        printf("(%d/%" PRIu16 ")\n", tam_recb, tamdatos);
    }

    return tam_recb;
}
