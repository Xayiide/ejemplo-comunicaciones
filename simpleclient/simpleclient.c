
#include <stdint.h>

#include "../include/apero.h"
#include "../include/comun.h"
#include "inc/simpleclient.h"

#include <stdio.h>
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
        printf("Todos los datos enviados correctamente ");
        printf("")
    }
#endif

    return ret;
}
