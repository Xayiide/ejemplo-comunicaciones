#include <stdio.h>  /* printf */
#include <unistd.h> /* close */

#include "inc/simpleserver.h"
#include "../include/comun.h"
#include "../include/apero.h"

#define MEDIO_SEGUNDO 500000

int main(void) {
    servidor *s = crearServidor(LISTENING_ADDR, LISTENING_PORT);

    if (s == NULL) {
        printf("Error creando servidor.\n");
        return -1;
    }

#ifdef DEBUG
    imprimirServidor(s);
#endif

    iniciarServidor(s, MAX_CONEXIONES);

    while (bucleConexiones(s) == 1) {
        usleep(MEDIO_SEGUNDO);
    }

    eliminarServidor(s);

    return 0;
}