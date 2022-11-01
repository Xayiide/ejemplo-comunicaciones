#include <stdio.h> /* printf */

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

    iniciarServidor(s, MAX_CONEXIONES);

    eliminarServidor(s);

    return 0;
}