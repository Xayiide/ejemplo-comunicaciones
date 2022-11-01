#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "inc/simpleserver.h"
#include "../include/comun.h"
#include "../include/apero.h"


int main(void) {
    servidor *s = crearServidor(LISTENING_ADDR, LISTENING_PORT);

    if (s == NULL) {
        printf("Error creando servidor.\n");
        return -1;
    }

    imprimirServidor(s);

    eliminarServidor(s);

    return 0;
}