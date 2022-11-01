
#include "../include/apero.h"
#include "../include/comun.h"
#include "inc/simpleserver.h"

#include <stdio.h>  /* perror  */
#include <stdint.h> /* uint8_t */


int iniciarServidor(servidor *s, uint8_t maxConexiones) {
    int ret;

    ret = listen(s->fd, maxConexiones);

    if (ret == -1) {
        perror("iniciarServidor");
    }

    return ret;
}

